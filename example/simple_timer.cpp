#include <iostream>
#include "../src/libsm.hpp"

enum State
{
    STATE,
};

enum Input
{
    MSG,
};

class Output : public sm::std::Router<Input, Output>::BasicOutput
{
public:
    Output() {}
};

typedef sm::SM<State, Input, Output> SM;
typedef sm::CTX<Input, Output> CTX;

class Context : public sm::std::Router<Input, Output>
{
public:
    virtual void send(void *sm, const O &o) override
    {
        std::cout << "timer event start" << o->get_target() << std::endl;
        sm::std::Router<Input, Output>::send(sm, o);
        std::cout << "timer event end" << std::endl;
    }
};

int main()
{
    std::unique_ptr<CTX> ctx = std::make_unique<Context>();
    std::unique_ptr<SM> sm = std::make_unique<sm::std::SimpleTimer<State, Input, Output>>(ctx);
    std::unique_ptr<Output> sms = std::make_unique<Context::Message>(Context::Message(sm.get(), Input()));
    ctx->send(sm.get(), sms);
    auto x = std::make_unique<State>(State::STATE);
    sm->run(std::move(x));
    return 0;
}