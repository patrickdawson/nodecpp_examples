#include <node.h>
#include <cstdio>

using namespace v8;

void PassNumber(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Local<Number> target = args[0]->ToNumber();
    double value = target->NumberValue();

    value += 42;

    Local<Number> retval = Number::New(isolate, value);
    args.GetReturnValue().Set(retval);
}

 void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "passNumber", PassNumber);
 }

 NODE_MODULE(addon, init)