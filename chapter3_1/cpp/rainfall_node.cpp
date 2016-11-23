#include <node.h>
#include <v8.h>
#include <vector>
#include <algorithm>

#include "rainfall.h"

using namespace v8;

sample unpack_sample(Isolate *isolate, const Local<Object> sample_obj)
{
    sample s;
    Local<Value> date_value = sample_obj->Get(String::NewFromUtf8(isolate, "date"));
    Local<Value> rainfall_value = sample_obj->Get(String::NewFromUtf8(isolate, "rainfall"));

    String::Utf8Value utfValue(date_value);
    s.date = std::string(*utfValue);
    // Unpack numeric rainfall amount from V8 value
    s.rainfall = rainfall_value->NumberValue();

    return s;
}

location unpack_location(Isolate* isolate, Local<Object> location_obj) {
    location loc;
    loc.latitude = location_obj->Get(String::NewFromUtf8(isolate, "latitude"))->NumberValue();
    loc.longitude = location_obj->Get(String::NewFromUtf8(isolate, "longitude"))->NumberValue();

    Local<Array> array = Local<Array>::Cast(location_obj->Get(String::NewFromUtf8(isolate, "samples")));

    int sample_count = array->Length();
    for (int i=0; i<sample_count; ++i) {
        sample s = unpack_sample(isolate, array->Get(i)->ToObject());
        loc.samples.push_back(s);
    }

    return loc;
}

void pack_rain_result(Isolate *isolate, Local<v8::Object> &target, rain_result &result) {
    target->Set(String::NewFromUtf8(isolate, "mean"), Number::New(isolate, result.mean));
    target->Set(String::NewFromUtf8(isolate, "median"), Number::New(isolate, result.median));
    target->Set(String::NewFromUtf8(isolate, "standard_deviation"), Number::New(isolate, result.standard_deviation));
    target->Set(String::NewFromUtf8(isolate, "n"), Integer::New(isolate, result.n));
}

void AvgRainfall(const FunctionCallbackInfo<Value> &args)
{
    Isolate* isolate = args.GetIsolate();

    location loc = unpack_location(isolate, args[0]->ToObject());
    double avg = avg_rainfall(loc);

    Local<Number> retval = Number::New(isolate, avg);
    args.GetReturnValue().Set(retval);
}

void RainfallData(const FunctionCallbackInfo<Value> &args) {
    Isolate* isolate = args.GetIsolate();

    location loc = unpack_location(isolate, args[0]->ToObject());
    rain_result result = calc_rain_stats(loc);

    Local<Object> obj = Object::New(isolate);

    obj->Set(
        String::NewFromUtf8(isolate, "mean"),
        Number::New(isolate, result.mean));
    obj->Set(
        String::NewFromUtf8(isolate, "median"),
        Number::New(isolate, result.median));
    obj->Set(
        String::NewFromUtf8(isolate, "standard_deviation"),
        Number::New(isolate, result.standard_deviation));
    obj->Set(
        String::NewFromUtf8(isolate, "n"),
        Number::New(isolate, result.n));

    args.GetReturnValue().Set(obj);
}

void CalculateResults(const FunctionCallbackInfo<Value> &args) {
    Isolate* isolate = args.GetIsolate();
    std::vector<location> locations;
    std::vector<rain_result> results;

    Local<Array> input = Local<Array>::Cast(args[0]);
    unsigned int num_locations = input->Length();

    for (unsigned int i = 0; i < num_locations; ++i)
    {
        locations.push_back(
            unpack_location(isolate, Local<Object>::Cast(input->Get(i))));
    }

    results.resize(locations.size());
    std::transform(
        locations.begin(),
        locations.end(),
        results.begin(),
        calc_rain_stats);

    Local<Array> result_list = Array::New(isolate);

    for (unsigned int i = 0; i < results.size(); i++)
    {
        Local<Object> result = Object::New(isolate);
        pack_rain_result(isolate, result, results[i]);
        result_list->Set(i, result);
    }

    args.GetReturnValue().Set(result_list);
}

void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "avg_rainfall", AvgRainfall);
    NODE_SET_METHOD(exports, "data_rainfall", RainfallData);
    NODE_SET_METHOD(exports, "calculate_results", CalculateResults);
}

NODE_MODULE(rainfall, init)