#include <node.h>

using namespace v8;

void CallThis(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Local<Function> cb = Local<Function>::Cast(args[0]);
    cb->Call(Null(isolate), 0, nullptr);

    return;
}

 void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "callthis", CallThis);
 }

 NODE_MODULE(callback, init)