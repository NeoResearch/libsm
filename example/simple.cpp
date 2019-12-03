#include <iostream>
#include "../src/libsm.hpp"

enum State
{
    INIT,
    FINAL,
};

enum Input
{
    MSG,
};

enum Output
{
    TRIG,
    LOG,
};

typedef sm::SM<State, Input, Output> SM;
typedef sm::CTX<Input, Output> CTX;

class StateMachine : public SM
{
public:
    StateMachine(C &ctx) : SM(ctx)
    {
    }

protected:
    virtual S tf(const I &i, const S &s) override
    {
        switch (*s)
        {
        case State::INIT:
            return std::make_unique<State>(State::FINAL);
        default:
            throw std::exception();
        }
    }
    virtual O of(const I &i, const S &s) override
    {
        switch (*s)
        {
        case State::INIT:
            return std::make_unique<Output>(Output::LOG);
        default:
            throw std::exception();
        }
    }
    virtual bool goon(const S &s) override
    {
        if (*s == State::INIT)
        {
            return true;
        }
        return false;
    }
};

class Context : public CTX
{
public:
    virtual void send(void *sm, const O &o) override
    {
        if (sm == nullptr && *o == Output::TRIG)
        {
            this->messages[target].push(std::make_unique<Input>(Input::MSG));
        }
        else if (*o == Output::LOG)
        {
            std::cout << "event triggered and print log" << std::endl;
        }
    }

    static void *target;
};

void *Context::target = nullptr;

int main()
{
    std::unique_ptr<CTX> ctx = std::make_unique<Context>();
    std::unique_ptr<SM> sm = std::make_unique<StateMachine>(ctx);
    Context::target = sm.get();
    ctx->send(nullptr, std::make_unique<Output>(Output::TRIG));
    sm->run(std::make_unique<State>(State::INIT));
    return 0;
}