#pragma once
#include <vector>
#include "../ctx.hpp"
namespace sm
{
namespace std
{
template <typename Input, typename Output>
class Event
{
public:
    Event() {}
    virtual void run(CTX<Input, Output> &ctx)
    {
    }
};
} // namespace std
} // namespace sm