#pragma once
#include <thread>
#include <chrono>
#include <vector>
#include "output.hpp"
#include "../ctx.hpp"
namespace sm
{
namespace std
{
template <typename Input>
class Router : public CTX<Input, Output<Input>>
{
public:
    using typename CTX<Input, Output<Input>>::I;
    using typename CTX<Input, Output<Input>>::O;

public:
    virtual void send(void *sm, const O &o) override
    {
        if (o == nullptr)
        {
            return;
        }
        for (auto &event : o->events())
        {
            event->run(*this);
        }
    }
};
} // namespace std
} // namespace sm
