#pragma once
#include "def.hpp"
#include "state.hpp"
#include "input.hpp"

struct Out;
struct O : public std::vector<Out>
{
};
struct Out
{
    enum
    {
        MESSAGE,
        LOG,
    } type;
    union {
        sm::std::Queue<S, I, O>::MESSAGE message;
    };
};