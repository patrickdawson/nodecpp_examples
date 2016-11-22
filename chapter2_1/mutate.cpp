#include <node.h>
#include <cstdio>

using namespace v8;

void Mutate(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Local<Object> target = args[0]->ToObject();
    target->Set(
        String::NewFromUtf8(isolate, "x"),
        Number::New(isolate, 42)
    );

    return;
}

 void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "mutate", Mutate);
 }

 NODE_MODULE(mutate, init)