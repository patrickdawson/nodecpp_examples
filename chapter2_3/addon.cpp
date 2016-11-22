#include <node.h>
#include <cstdio>

using namespace v8;

Local<Value> MakeReturn(Isolate* isolate, const Local<Object> input) {
    EscapableHandleScope scope(isolate);
    //HandleScope scope(isolate);
    Local<String> x_prop = String::NewFromUtf8(isolate, "x");
    Local<String> y_prop = String::NewFromUtf8(isolate, "y");
    Local<String> sum_prop = String::NewFromUtf8(isolate, "sum");
    Local<String> product_prop = String::NewFromUtf8(isolate, "product");

    double x = input->Get(x_prop)->NumberValue();
    double y = input->Get(y_prop)->NumberValue();

    Local<Object> obj = Object::New(isolate);
    obj->Set(sum_prop, Number::New(isolate, x + y));
    obj->Set(product_prop, Number::New(isolate, x * y));

    return scope.Escape(obj);
    //return obj;
}

void PassObject(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Local<Value> target = MakeReturn(isolate, args[0]->ToObject());

    args.GetReturnValue().Set(target);
}

void init(Local<Object> exports)
{
    NODE_SET_METHOD(exports, "passObject", PassObject);
 }

 NODE_MODULE(addon, init)