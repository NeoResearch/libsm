#pragma once

#include "ctx.hpp"
namespace sm
{
template <typename State, typename Input, typename Output>
class SM
{
public:
    typedef std::unique_ptr<State> S;
    typedef std::unique_ptr<Input> I;
    typedef std::unique_ptr<Output> O;
    typedef std::unique_ptr<CTX<Input, Output>> C;

public:
    SM(C &ctx) : ctx(ctx)
    {
    }

    void run(S &&initial_state)
    {
        for (auto state = std::move(initial_state); goon(state);)
        {
            auto input = ctx->recv(this);
            auto next = tf(input, state);
            auto output = of(input, state);
            ctx->send(this, output);
            state.swap(next);
        }
    }

protected:
    virtual S tf(const I &i, const S &s)
    {
        throw std::exception();
    }
    virtual O of(const I &i, const S &s)
    {
        throw std::exception();
    }
    virtual bool goon(const S &s)
    {
        return true;
    }

private:
    const C &ctx;
};
} // namespace sm