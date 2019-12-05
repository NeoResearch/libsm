#pragma once
#include "def.hpp"
struct I
{
    enum
    {
        REQUEST,
    } type;
    union {
        struct
        {
            enum
            {
                INC,
            } o;
            unsigned t;
            unsigned c;
        } request;
    };
    friend std::ostream &operator<<(std::ostream &os, const I &i)
    {
        os << "[INPUT]";
        return os;
    }
};