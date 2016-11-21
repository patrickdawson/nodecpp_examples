#include <node.h>

using namespace v8;

void Add(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    if (args.Length() < 2) {
        //isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate, "Wrong number of arguments")));
        return;
    }
    if (!args[0]->IsNumber() || !args[1]->IsNumber()) {
        return; // undefined goes back to the caller
    }

    double value = args[0]->NumberValue() + args[1]->NumberValue();
    Local<Number> result = Number::New(isolate, value);

    args.GetReturnValue().Set(result);
}

 void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "add", Add);
 }

 NODE_MODULE(cmath, init)