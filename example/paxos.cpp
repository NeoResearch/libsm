#include <iostream>
#include <vector>
#include <thread>
#include <sstream>
#include "../src/libsm.hpp"

const int f = 1;

struct State
{
    enum
    {
        PROPOSER,
        ACCEPTOR,
        LEARNER,
    } value;
    struct proposer
    {
        unsigned n;
        unsigned m;
        unsigned p;
        unsigned x;
    } proposer;
    struct acceptor
    {
        unsigned n;
        unsigned m;
        unsigned v;
    } acceptor;
    struct learner
    {
        unsigned v;
    } learner;
};

struct Input
{
    enum
    {
        REQUEST,
        PREPARE,
        PROMISE,
        ACCEPT,
        ACCEPTED,
    } value;
    struct request
    {
        unsigned x;
    } request;
    struct prepare
    {
        unsigned n;
    } prepare;
    struct promise
    {
        unsigned n;
        unsigned m;
        unsigned w;
    } promise;
    struct accept
    {
        unsigned n;
        unsigned v;
    } accept;
    struct accepted
    {
        unsigned n;
        unsigned v;
    } accepted;
};

class Output : public sm::std::MultiRouter<Input, Output>::BasicOutput
{
};

typedef sm::SM<State, Input, Output> SM;
typedef sm::CTX<Input, Output> CTX;

std::ostream &operator<<(std::ostream &os, const State &s)
{
    switch (s.value)
    {
    case State::PROPOSER:
        return os << "<PROPOSER>: "
                  << " n=" << s.proposer.n
                  << " m=" << s.proposer.m
                  << " p=" << s.proposer.p
                  << " x=" << s.proposer.x;
    case State::ACCEPTOR:
        return os << "<ACCEPTOR>: "
                  << " n=" << s.acceptor.n
                  << " m=" << s.acceptor.m
                  << " v=" << s.acceptor.v;
    case State::LEARNER:
        return os << "<LEARNER>: "
                  << " v=" << s.learner.v;
    default:
        return os << "<UNKNOWN>: ";
    }
}

std::ostream &operator<<(std::ostream &os, const Input &s)
{
    switch (s.value)
    {
    case Input::REQUEST:
        return os << "<REQUEST>: "
                  << " x=" << s.request.x;
    case Input::PREPARE:
        return os << "<PREPARE>: "
                  << " n=" << s.prepare.n;
    case Input::PROMISE:
        return os << "<PROMISE>: "
                  << " n=" << s.promise.n
                  << " m=" << s.promise.m
                  << " w=" << s.promise.w;
    case Input::ACCEPT:
        return os << "<ACCEPT>: "
                  << " n=" << s.accept.n
                  << " v=" << s.accept.v;
    case Input::ACCEPTED:
        return os << "<ACCEPTED>: "
                  << " n=" << s.accepted.n
                  << " v=" << s.accepted.v;
    default:
        return os << "<UNKNOWN>: ";
    }
}

class Context : public sm::std::MultiRouter<Input, Output>
{
public:
    virtual void send(void *sm, const O &o) override
    {
        sm::std::MultiRouter<Input, Output>::send(sm, o);
    }

public:
    static std::vector<void *> acceptors;
    static void *proposer;
    static void *learner;
};

class StateMachine : public SM
{
public:
    StateMachine(C &ctx) : SM(ctx)
    {
    }

protected:
    virtual S tf(const I &i, const S &s) override
    {
        std::cout << "[TF][FROM]: " << this << " " << *i << " " << *s << std::endl;
        S ret = std::make_unique<State>(*s);
        if (i->value == Input::REQUEST)
        {
            if (s->value == State::PROPOSER)
            {
                ret->proposer.n = ret->proposer.n + 1;
                ret->proposer.p = 0;
                ret->proposer.x = i->request.x;
            }
        }
        else if (i->value == Input::PREPARE)
        {
            if (s->value == State::ACCEPTOR)
            {
                if (s->acceptor.n < i->prepare.n)
                {
                    ret->acceptor.n = i->prepare.n;
                }
            }
        }
        else if (i->value == Input::PROMISE)
        {
            if (s->value == State::PROPOSER)
            {
                if (s->proposer.n == i->promise.n)
                {
                    if (s->proposer.m < i->promise.m)
                    {
                        ret->proposer.m = i->promise.m;
                        ret->proposer.x = i->promise.w;
                        ret->proposer.p = ret->proposer.p + 1;
                    }
                    else
                    {
                        ret->proposer.p = ret->proposer.p + 1;
                    }
                }
            }
        }
        else if (i->value == Input::ACCEPT)
        {
            if (s->value == State::ACCEPTOR)
            {
                if (s->acceptor.n <= i->accept.n)
                {
                    ret->acceptor.m = i->accept.n;
                    ret->acceptor.v = i->accept.v;
                }
            }
        }
        else if (i->value == Input::ACCEPTED)
        {
            if (s->value == State::LEARNER)
            {
                ret->learner.v = i->accepted.v;
            }
        }
        std::cout << "[TF][TO]: " << this << " " << *ret << std::endl;
        return ret;
    }
    virtual O of(const I &i, const S &s) override
    {
        if (i->value == Input::REQUEST)
        {
            if (s->value == State::PROPOSER)
            {
                Input input;
                input.value = Input::PREPARE;
                input.prepare.n = s->proposer.n + 1;
                return std::make_unique<Context::Message>(Context::Message(Context::acceptors, input));
            }
        }
        else if (i->value == Input::PREPARE)
        {
            if (s->value == State::ACCEPTOR)
            {
                if (s->acceptor.n < i->prepare.n)
                {
                    Input input;
                    input.value = Input::PROMISE;
                    input.promise.n = i->prepare.n;
                    input.promise.m = s->acceptor.m;
                    input.promise.w = s->acceptor.v;
                    return std::make_unique<Context::Message>(Context::Message({Context::proposer}, input));
                }
            }
        }
        else if (i->value == Input::PROMISE)
        {
            if (s->value == State::PROPOSER)
            {
                if (s->proposer.n == i->promise.n)
                {
                    if (s->proposer.p == f)
                    {
                        Input input;
                        input.value = Input::ACCEPT;
                        input.accept.n = s->proposer.n;
                        input.accept.v = s->proposer.x;
                        return std::make_unique<Context::Message>(Context::Message(Context::acceptors, input));
                    }
                }
            }
        }
        else if (i->value == Input::ACCEPT)
        {
            if (s->value == State::ACCEPTOR)
            {
                if (s->acceptor.n <= i->accept.n)
                {
                    Input input;
                    input.value = Input::ACCEPTED;
                    input.accepted.n = i->accept.n;
                    input.accepted.v = i->accept.v;
                    return std::make_unique<Context::Message>(Context::Message({Context::proposer, Context::learner}, input));
                }
            }
        }
        return nullptr;
    }
    virtual bool goon(const S &s) override
    {
        if (s == nullptr)
        {
            return false;
        }
        return true;
    }
};

std::vector<void *> Context::acceptors = std::vector<void *>({nullptr, nullptr, nullptr});
void *Context::proposer = nullptr;
void *Context::learner = nullptr;

int main()
{
    std::unique_ptr<CTX> ctx = std::make_unique<Context>();
    std::unique_ptr<SM> sm_proposer = std::make_unique<StateMachine>(ctx);
    std::unique_ptr<SM> sm_acceptor1 = std::make_unique<StateMachine>(ctx);
    std::unique_ptr<SM> sm_acceptor2 = std::make_unique<StateMachine>(ctx);
    std::unique_ptr<SM> sm_acceptor3 = std::make_unique<StateMachine>(ctx);
    std::unique_ptr<SM> sm_learner = std::make_unique<StateMachine>(ctx);

    Context::proposer = sm_proposer.get();
    Context::acceptors[0] = sm_acceptor1.get();
    Context::acceptors[1] = sm_acceptor2.get();
    Context::acceptors[2] = sm_acceptor3.get();
    Context::learner = sm_learner.get();

    Input input;
    input.value = Input::REQUEST;
    input.request.x = 517;
    std::unique_ptr<Output> msg = std::make_unique<Context::Message>(Context::Message({sm_proposer.get()}, input));
    ctx->send(nullptr, msg);
    std::thread thread_propopser([&]() {
        State state;
        state.value = State::PROPOSER;
        state.proposer.n = 1;
        state.proposer.m = 0;
        state.proposer.p = 0;
        state.proposer.x = 0;
        sm_proposer->run(std::make_unique<State>(state));
    });
    std::thread thread_acceptor1([&]() {
        State state;
        state.value = State::ACCEPTOR;
        state.acceptor.n = 0;
        state.acceptor.m = 0;
        state.acceptor.v = 0;
        sm_acceptor1->run(std::make_unique<State>(state));
    });
    std::thread thread_acceptor2([&]() {
        State state;
        state.value = State::ACCEPTOR;
        state.acceptor.n = 0;
        state.acceptor.m = 0;
        state.acceptor.v = 0;
        sm_acceptor2->run(std::make_unique<State>(state));
    });
    std::thread thread_acceptor3([&]() {
        State state;
        state.value = State::ACCEPTOR;
        state.acceptor.n = 0;
        state.acceptor.m = 0;
        state.acceptor.v = 0;
        sm_acceptor3->run(std::make_unique<State>(state));
    });
    std::thread thread_learner([&]() {
        State state;
        state.value = State::LEARNER;
        state.learner.v = 0;
        sm_learner->run(std::make_unique<State>(state));
    });
    thread_propopser.join();
    return 0;
}