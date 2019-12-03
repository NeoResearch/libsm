#pragma once

#include <map>
#include "lib.hpp"

namespace sm
{
template <typename Input, typename Output>
class CTX
{
public:
    typedef std::unique_ptr<Input> I;
    typedef std::unique_ptr<Output> O;

public:
    CTX()
    {
    }

    I recv(void *sm)
    {
        return messages[sm].pop();
    }

    virtual void send(void *sm, const O &o)
    {
    }

protected:
    std::map<void *, lib::queue<I>> messages;
};
} // namespace sm