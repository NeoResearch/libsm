#pragma once

#include <thread>
#include <chrono>
#include <vector>
#include "../sm.hpp"

namespace sm
{
namespace std
{
template <typename Input, typename Output>
class Router : public CTX<Input, Output>
{
public:
    typedef typename CTX<Input, Output>::I I;
    typedef typename CTX<Input, Output>::O O;

public:
    class Message : public Output
    {
    public:
        Message(void *target, Input input) : target(target), input(input)
        {
        }

        virtual void *get_target() const override
        {
            return target;
        }

        virtual I get_input() const override
        {
            return ::std::make_unique<Input>(input);
        }

        void *target;
        Input input;
    };

    class BasicOutput
    {
    public:
        virtual void *get_target() const
        {
            return nullptr;
        }
        virtual I get_input() const
        {
            return nullptr;
        }
    };

    virtual void send(void *sm, const O &o) override
    {
        if (o == nullptr)
        {
            return;
        }
        if (o->get_target() == nullptr)
        {
            return;
        }
        this->messages[o->get_target()].push(::std::move(o->get_input()));
    }
};

template <typename Input, typename Output>
class MultiRouter : public CTX<Input, Output>
{
public:
    typedef typename CTX<Input, Output>::I I;
    typedef typename CTX<Input, Output>::O O;

public:
    class Message : public Output
    {
    public:
        Message(const ::std::vector<void *> &target, Input input) : target(target), input(input)
        {
        }

        virtual ::std::vector<void *> get_target() const override
        {
            return target;
        }

        virtual I get_input() const override
        {
            return ::std::make_unique<Input>(input);
        }

        ::std::vector<void *> target;
        Input input;
    };

    class BasicOutput
    {
    public:
        virtual ::std::vector<void *> get_target() const
        {
            return ::std::vector<void *>();
        }
        virtual I get_input() const
        {
            return nullptr;
        }
    };

    virtual void send(void *sm, const O &o) override
    {
        if (o == nullptr)
        {
            return;
        }
        for (auto k : o->get_target())
        {
            this->messages[k].push(::std::move(o->get_input()));
        }
    }
};

} // namespace std
} // namespace sm
