#include <iostream>
#include "../../src/libsm.hpp"

enum State
{
    STATE,
};

enum Input
{
    MSG,
};

typedef sm::std::Output<Input> Output;

typedef sm::SM<State, Input, Output> SM;
typedef sm::CTX<Input, Output> CTX;

class Context : public sm::std::Router<Input>
{
public:
    virtual void send(void *sm, const O &o) override
    {
        std::cout << "timer event start" << std::endl;
        sm::std::Router<Input>::send(sm, o);
        std::cout << "timer event end" << std::endl;
    }
};

int main()
{
    std::unique_ptr<CTX> ctx = std::make_unique<Context>();
    std::unique_ptr<SM> sm = std::make_unique<sm::std::SimpleTimer<State, Input, Output>>(ctx);
    ctx->msg(sm.get(), std::make_unique<Input>());
    auto x = std::make_unique<State>(State::STATE);
    sm->run(std::move(x));
    return 0;
}