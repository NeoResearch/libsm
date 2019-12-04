#include <iostream>
#include "../../../src/sm.hpp"

enum S
{
    INIT,
    FINAL,
};

enum I
{
    MSG,
};

enum O
{
    LOG,
};

class StateMachine : public sm::SM<S, I, O>
{
public:
    StateMachine(RECV recv, SEND send) : SM(recv, send)
    {
    }

protected:
    virtual S tf(const I &i, const S &s) override
    {
        if (i == I::MSG && s == S::INIT)
        {
            return S::FINAL;
        }
        else
        {
            throw std::exception();
        }
    }
    virtual O of(const I &i, const S &s) override
    {
        if (i == I::MSG && s == S::INIT)
        {
            return O::LOG;
        }
        else
        {
            throw std::exception();
        }
    }
    virtual bool goon(const S &s) override
    {
        if (s == S::INIT)
        {
            return true;
        }
        return false;
    }
};

int main()
{
    auto sm = StateMachine([] { return I::MSG; }, [](const O &) { std::cout << "ok" << std::endl; });
    sm.run(S::INIT);
    return 0;
}