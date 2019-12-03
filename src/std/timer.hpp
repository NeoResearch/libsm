#pragma once

#include <thread>
#include <chrono>
#include "../sm.hpp"
#include "router.hpp"

namespace sm
{
namespace std
{

template <typename State,
          typename Input,
          typename Output,
          unsigned second>
class Timer : public SM<State, Input, Output>
{
public:
    using typename SM<State, Input, Output>::S;
    using typename SM<State, Input, Output>::I;
    using typename SM<State, Input, Output>::O;
    using typename SM<State, Input, Output>::C;

public:
    Timer(C &ctx) : SM<State, Input, Output>(ctx)
    {
    }

protected:
    virtual S tf(const I &i, const S &s) override
    {
        ::std::this_thread::sleep_for(::std::chrono::seconds(second));
        return ::std::make_unique<State>(*s);
    }
    virtual O of(const I &i, const S &s) override
    {
        O ret = ::std::make_unique<Output>();
        ret->push(::std::make_unique<Message<Input, Output>>(this, ::std::make_unique<Input>()));
        return ret;
    }
    virtual bool goon(const S &s) override
    {
        return true;
    }
};

template <typename State,
          typename Input,
          typename Output>
using SimpleTimer = Timer<State, Input, Output, 1>;

} // namespace std
} // namespace sm
