#include <node.h>
#include <v8.h>

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

location unpack_location(Isolate* isolate, const FunctionCallbackInfo<Value>& args) {
    location loc;
    Local<Object> location_obj = args[0]->ToObject();
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

void AvgRainfall(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    location loc = unpack_location(isolate, args);
    double avg = avg_rainfall(loc);

    Local<Number> retval = Number::New(isolate, avg);
    args.GetReturnValue().Set(retval);
}

void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "avg_rainfall", AvgRainfall);
}

NODE_MODULE(rainfall, init)