#include <node.h>
#include <cstdio>

using namespace v8;

void PassNumber(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    // This would catch anything that can't be a Number
    /*if ( !args[0]->IsNumber()) {
        args.GetReturnValue().Set(Number::New(isolate, -1));
        return;
    }*/

    Local<Number> target = args[0]->ToNumber();
    double value = target->NumberValue();

    value += 42;

    Local<Number> retval = Number::New(isolate, value);
    args.GetReturnValue().Set(retval);
}

void PassInteger(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Local<Int32> target = args[0]->ToInt32();
    int value = target->NumberValue();

    Local<Number> retval = Number::New(isolate, value);
    args.GetReturnValue().Set(retval);
}

void PassBoolean(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Local<Boolean> target = args[0]->ToBoolean();
    bool value = target->BooleanValue();

    Local<Number> retval = Number::New(isolate, value);
    args.GetReturnValue().Set(retval);
}

 void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "passNumber", PassNumber);
    NODE_SET_METHOD(exports, "passInteger", PassInteger);
    NODE_SET_METHOD(exports, "passBoolean", PassBoolean);
 }

 NODE_MODULE(addon, init)