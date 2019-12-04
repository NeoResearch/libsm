#pragma once
#include <functional>
namespace sm
{
template <typename S, typename I, typename O>
class SM
{
public:
    using RECV = std::function<I()>;
    using SEND = std::function<void(const O &)>;
    SM(RECV recv, SEND send) : recv(recv), send(send) {}

    void run(S initial)
    {
        for (auto state = initial; goon(state); state = step(state))
        {
        }
    }

protected:
    virtual S tf(const I &i, const S &s)
    {
        return s;
    }
    virtual O of(const I &i, const S &s)
    {
        return O();
    }
    virtual bool goon(const S &s)
    {
        return false;
    }

private:
    RECV recv;
    SEND send;
    S step(const S &s)
    {
        auto input = recv();
        auto next = tf(input, s);
        auto output = of(input, s);
        send(output);
        return next;
    }
};
} // namespace sm