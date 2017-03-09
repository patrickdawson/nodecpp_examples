#include <node.h>
#include <v8.h>
#include <uv.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>

#include "rainfall.h"

using namespace v8;

struct Work
{
    uv_work_t request;
    Persistent<Function> callback;

    std::vector<location> locations;
    std::vector<rain_result> results;
};

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

void pack_rain_result(Isolate *isolate, Local<Object> &target, rain_result &result) {
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
            //unpack_location(isolate, Local<Object>::Cast(input->Get(i))));
            unpack_location(isolate, input->Get(i)->ToObject()));
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

void CalculateResultsSync(const FunctionCallbackInfo<Value>& args) {
    Isolate *isolate = args.GetIsolate();
    std::vector<location> locations;
    std::vector<rain_result> results;

    Local<Array> input = Local<Array>::Cast(args[0]);
    unsigned int num_locations = input->Length();

    for (unsigned int i = 0; i < num_locations; ++i)
    {
        locations.push_back(
            //unpack_location(isolate, Local<Object>::Cast(input->Get(i))));
            unpack_location(isolate, input->Get(i)->ToObject()));
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

    Local<Function> callback = Local<Function>::Cast(args[1]);
    Local<Value> argv[] = { result_list };
    callback->Call(Null(isolate), 1, argv);

    std::cerr << "Returning from c++ now" << std::endl;

    args.GetReturnValue().Set(Undefined(isolate));
}

static void WorkAsync(uv_work_t* req) {
    Work* work = static_cast<Work*>(req->data);

    work->results.resize(work->locations.size());
    std::transform(
        work->locations.begin(),
        work->locations.end(),
        work->results.begin(),
        calc_rain_stats);

    // simulate long running process
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

static void WorkAsyncComplete(uv_work_t* req, int status) {
    Isolate* isolate = Isolate::GetCurrent();
    HandleScope handleScope(isolate);

    Work* work = static_cast<Work*>(req->data);

    Local<Array> result_list = Array::New(isolate);
    for (unsigned int i=0; i<work->results.size(); ++i) {
        Local<Object> result = Object::New(isolate);
        pack_rain_result(isolate, result, work->results[i]);
        result_list->Set(i, result);
    }

    // Set up return argument
    Local<Value> argv[] = { Null(isolate), result_list };
    Local<Function> cb = Local<Function>::New(isolate, work->callback);
    cb->Call(isolate->GetCurrentContext()->Global(), 2, argv);

    work->callback.Reset(),
    delete work;
}

void CalculateResultsAsync(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Work* work = new Work();
    work->request.data = work;

    Local<Array> input = Local<Array>::Cast(args[0]);
    unsigned int num_locations = input->Length();
    for (unsigned int i = 0; i<num_locations; ++i)
    {        
        work->locations.push_back(unpack_location(isolate, Local<Object>::Cast(input->Get(i))));
    }

    Local<Function> callback = Local<Function>::Cast(args[1]);
    work->callback.Reset(isolate, callback);

    // kick off worker thread
    uv_queue_work(uv_default_loop(), &work->request, WorkAsync, WorkAsyncComplete);
    
    args.GetReturnValue().Set(Undefined(isolate));
}

void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "avg_rainfall", AvgRainfall);
    NODE_SET_METHOD(exports, "data_rainfall", RainfallData);
    NODE_SET_METHOD(exports, "calculate_results", CalculateResults);
    NODE_SET_METHOD(exports, "calculate_results_sync", CalculateResultsSync);
    NODE_SET_METHOD(exports, "calculate_results_async", CalculateResultsAsync);
}

NODE_MODULE(rainfall, init)