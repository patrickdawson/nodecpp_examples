#include <node.h>
#include <string>

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

void PassString(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    String::Utf8Value s(args[0]);

    // wrap in c++ string instead
    std::string str(*s);
    std::reverse(str.begin(), str.end());

    Local<String> retval = String::NewFromUtf8(isolate, str.c_str());
    args.GetReturnValue().Set(retval);
}

void PassObject(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Local<Object> target = args[0]->ToObject();
    target->Set(String::NewFromUtf8(isolate, "y"), Number::New(isolate, 10));

    args.GetReturnValue().Set(target);
}

void PassObject2(const FunctionCallbackInfo<Value>& args) {
    Isolate* isolate = args.GetIsolate();

    Local<Object> target = args[0]->ToObject();
    Local<String> x_prop = String::NewFromUtf8(isolate, "x");
    Local<String> y_prop = String::NewFromUtf8(isolate, "y");
    Local<String> sum_prop = String::NewFromUtf8(isolate, "sum");
    Local<String> product_prop = String::NewFromUtf8(isolate, "product");

    double x = target->Get(x_prop)->NumberValue();
    double y = target->Get(y_prop)->NumberValue();

    Local<Object> obj = Object::New(isolate);
    obj->Set(sum_prop, Number::New(isolate, x+y));
    obj->Set(product_prop, Number::New(isolate, x*y));

    args.GetReturnValue().Set(obj);
}

 void init(Local<Object> exports) {
    NODE_SET_METHOD(exports, "passNumber", PassNumber);
    NODE_SET_METHOD(exports, "passInteger", PassInteger);
    NODE_SET_METHOD(exports, "passBoolean", PassBoolean);
    NODE_SET_METHOD(exports, "passString", PassString);
    NODE_SET_METHOD(exports, "passObject", PassObject);
    NODE_SET_METHOD(exports, "passObject2", PassObject2);
 }

 NODE_MODULE(addon, init)