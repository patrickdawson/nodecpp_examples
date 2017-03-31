#include <node.h>
#include <node_object_wrap.h>

class WrappedPoly : public node::ObjectWrap
{
public:
    static void Init(v8::Local<v8::Object> exports);

private:
    explicit WrappedPoly(double a = 0, double b = 0, double c = 0);
    ~WrappedPoly();

    static void New(const v8::FunctionCallbackInfo<v8::Value>& args);

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

void InitPoly(v8::Local<v8::Object> exports)
{
    WrappedPoly::Init(exports);
}

NODE_MODULE(polynomial, InitPoly);
