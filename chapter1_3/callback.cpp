#include <node.h>
#include <cstdio>

using namespace v8;

void CallThis(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Local<Function> cb = Local<Function>::Cast(args[0]);
    if (args.Length() > 1) {
        Local<Value> argv[1] = {args[1]};
        cb->Call(Null(isolate), 1, argv);
    } else {
        cb->Call(Null(isolate), 0, nullptr);
    }

    return;
}

 void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "callthis", CallThis);
 }

 NODE_MODULE(callback, init)