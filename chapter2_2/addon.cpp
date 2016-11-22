#include <node.h>
#include <cstdio>

using namespace v8;

void Func(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    return;
}

 void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "func", Func);
 }

 NODE_MODULE(xxxx, init)