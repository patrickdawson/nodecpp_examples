#include <node.h>
#include <v8.h>

#include "rainfall.h"

using namespace v8;

void AvgRainfall(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Local<Number> retval = Number::New(isolate, 0);
    args.GetReturnValue().Set(retval);
}

void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "avg_rainfall", AvgRainfall);
}

NODE_MODULE(rainfall, init)