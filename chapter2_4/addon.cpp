#include <node.h>
#include <cstdio>

using namespace v8;

Persistent<Object> persist;
int increment;

void Increment(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    Local<Object> target = Local<Object>::New(isolate, persist);
    Local<String> prop = String::NewFromUtf8(isolate, "x");
    double x = target->Get(prop)->NumberValue();
    target->Set(prop, Number::New(isolate, x + increment));
}

void Initialize(const FunctionCallbackInfo<Value> &args)
{
    Isolate *isolate = args.GetIsolate();
    // set global variables so they survive returning from this call
    persist.Reset(isolate, args[0]->ToObject());
    increment = args[1]->NumberValue();
}

void init(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "init", Initialize);
    NODE_SET_METHOD(exports, "increment", Increment);
}

 NODE_MODULE(addon, init)