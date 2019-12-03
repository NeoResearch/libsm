#pragma once
#include <vector>
#include "event.hpp"
namespace sm
{
namespace std
{
template <typename Input, typename Output>
class Message : public Event<Input, Output>
{
public:
    Message(void *target, ::std::unique_ptr<Input> &&i) : target(target), i(::std::move(i)) {}
    virtual void run(CTX<Input, Output> &ctx) override
    {
        ctx.msg(target, ::std::move(i));
    }

private:
    void *target;
    ::std::unique_ptr<Input> i;
};
} // namespace std
} // namespace sm