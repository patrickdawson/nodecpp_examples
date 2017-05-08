#include <node.h>
#include <node_object_wrap.h>
#include <math.h>
#include <string>

class WrappedPoly : public node::ObjectWrap
{
public:
    static void Init(v8::Local<v8::Object> exports);

private:
    explicit WrappedPoly(double a = 0, double b = 0, double c = 0);
    ~WrappedPoly();

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void At(const v8::FunctionCallbackInfo<v8::Value>& args);
    static void Roots(const v8::FunctionCallbackInfo<v8::Value>& args);

    static void GetCoeff(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info);
    static void SetCoeff(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info);

    static v8::Persistent<v8::Function> constructor;
    double a_;
    double b_;
    double c_;
};

v8::Persistent<v8::Function> WrappedPoly::constructor;

void WrappedPoly::Init(v8::Local<v8::Object> exports)
{
    v8::Isolate* isolate = exports->GetIsolate();

    // Prepare constructor template
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate, New);

    // Care that objects show up with correct name in javascript.
    tpl->SetClassName(v8::String::NewFromUtf8(isolate, "Polynomial"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    NODE_SET_PROTOTYPE_METHOD(tpl, "at", At);
    NODE_SET_PROTOTYPE_METHOD(tpl, "roots", Roots);

    tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "a"), GetCoeff, SetCoeff);
    tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "b"), GetCoeff, SetCoeff);
    tpl->InstanceTemplate()->SetAccessor(v8::String::NewFromUtf8(isolate, "c"), GetCoeff, SetCoeff);

    constructor.Reset(isolate, tpl->GetFunction());
    exports->Set(v8::String::NewFromUtf8(isolate, "Polynomial"), tpl->GetFunction());
}

WrappedPoly::WrappedPoly(double a /* = 0 */, double b /* = 0 */, double c /* = 0 */)
{

}

WrappedPoly::~WrappedPoly()
{

}

void WrappedPoly::New(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    auto isolate = args.GetIsolate();

    if (args.IsConstructCall()) {
        auto a = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
        auto b = args[1]->IsUndefined() ? 0 : args[1]->NumberValue();
        auto c = args[2]->IsUndefined() ? 0 : args[2]->NumberValue();

        auto obj = new WrappedPoly(a, b, c);
        obj->Wrap(args.This());
        args.GetReturnValue().Set(args.This());
    } else {
        // Invoked as plain function. Turn into construct call.
        const int argc = 3;
        v8::Local<v8::Value> argv[argc] = { args[0], args[1], args[2] };
        v8::Local<v8::Function> cons = v8::Local<v8::Function>::New(isolate, constructor);
        args.GetReturnValue().Set(cons->NewInstance(argc, argv));
    }
}

void WrappedPoly::At(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = args.GetIsolate();
    double x = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    WrappedPoly* poly = ObjectWrap::Unwrap<WrappedPoly>(args.Holder());

    double results = x * x * poly->a_ + x * poly->b_ + poly->c_;
    args.GetReturnValue().Set(v8::Number::New(isolate, results));
}

void WrappedPoly::Roots(const v8::FunctionCallbackInfo<v8::Value>& args)
{
    v8::Isolate* isolate = args.GetIsolate();
    WrappedPoly* poly = ObjectWrap::Unwrap<WrappedPoly>(args.Holder());

    v8::Local<v8::Array> roots = v8::Array::New(isolate);
    double desc = poly->b_ * poly->b_ - (4 * poly->a_ * poly->c_);
    if (desc >= 0) {
        double r = (-poly->b_ + sqrt(desc)) / (2 * poly->a_);
        roots->Set(0, v8::Number::New(isolate, r));
        if (desc > 0) {
            r = (-poly->b_ - sqrt(desc)) / (2 * poly->a_);
            roots->Set(1, v8::Number::New(isolate, r));
        }
    }
    args.GetReturnValue().Set(roots);
}

void WrappedPoly::GetCoeff(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    v8::Isolate* isolate = info.GetIsolate();
    WrappedPoly* poly = ObjectWrap::Unwrap<WrappedPoly>(info.This());

    v8::String::Utf8Value s(property);
    std::string str(*s);

    if (str == "a")
    {
        info.GetReturnValue().Set(v8::Number::New(isolate, poly->a_));
    }
    else if (str == "b")
    {
        info.GetReturnValue().Set(v8::Number::New(isolate, poly->b_));
    }
    else if (str == "c")
    {
        info.GetReturnValue().Set(v8::Number::New(isolate, poly->c_));
    }
}

void WrappedPoly::SetCoeff(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    v8::Isolate* isolate = info.GetIsolate();
    WrappedPoly* poly = ObjectWrap::Unwrap<WrappedPoly>(info.This());

    v8::String::Utf8Value s(property);
    std::string str(*s);

    if (str == "a")
    {
        poly->a_ = value->NumberValue();
    }
    else if (str == "b")
    {
        poly->b_ = value->NumberValue();
    }
    else if (str == "c")
    {
        poly->c_ = value->NumberValue();
    }
}

void InitPoly(v8::Local<v8::Object> exports)
{
    WrappedPoly::Init(exports);
}

NODE_MODULE(polynomial, InitPoly);
