#pragma once
#include "def.hpp"
#include "input.hpp"
struct S
{
    unsigned index;
    int state;
    std::vector<I> message_log;
    unsigned current_view;
    friend std::ostream &operator<<(std::ostream &os, const S &s)
    {
        os << "[index=" << s.index << "]"
           << "[state=" << s.state << "]"
           << "[|message_log|]=" << s.message_log.size() << "]"
           << "[current_view=" << s.current_view << "]";
        return os;
    }
};
