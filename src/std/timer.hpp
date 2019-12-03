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
    typedef typename SM<State, Input, Output>::S S;
    typedef typename SM<State, Input, Output>::I I;
    typedef typename SM<State, Input, Output>::O O;
    typedef typename SM<State, Input, Output>::C C;

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
        return ::std::make_unique<typename Router<Input, Output>::Message>(this, Input());
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