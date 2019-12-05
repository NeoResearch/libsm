#include <iostream>
#include <array>
#include <vector>
#include <thread>
#include "../../../src/std.hpp"

class SM;

struct GLOBAL
{
    static const int F = 1;
    static std::array<SM, 1> smsp;
    static std::array<SM, 3> smsa;
    static std::array<SM, 1> smsl;
};

struct S
{
    enum
    {
        PROPOSER,
        ACCEPTOR,
        LEARNER,
    } type;
    union {
        struct
        {
            unsigned n;
            unsigned m;
            unsigned p;
            unsigned x;
        } proposer;
        struct
        {
            unsigned n;
            unsigned m;
            unsigned v;
        } acceptor;
        struct
        {
            unsigned v;
        } learner;
    };
    friend std::ostream &operator<<(std::ostream &os, const S &s)
    {
        switch (s.type)
        {
        case S::PROPOSER:
            return os << "<PROPOSER>: "
                      << " n=" << s.proposer.n
                      << " m=" << s.proposer.m
                      << " p=" << s.proposer.p
                      << " x=" << s.proposer.x;
        case S::ACCEPTOR:
            return os << "<ACCEPTOR>: "
                      << " n=" << s.acceptor.n
                      << " m=" << s.acceptor.m
                      << " v=" << s.acceptor.v;
        case S::LEARNER:
            return os << "<LEARNER>: "
                      << " v=" << s.learner.v;
        default:
            return os << "<UNKNOWN>: ";
        }
    }
};

struct I
{
    enum
    {
        REQUEST,
        PREPARE,
        PROMISE,
        ACCEPT,
        ACCEPTED,
    } type;
    union {
        struct
        {
            unsigned x;
        } request;
        struct
        {
            unsigned n;
        } prepare;
        struct
        {
            unsigned n;
            unsigned m;
            unsigned w;
        } promise;
        struct
        {
            unsigned n;
            unsigned v;
        } accept;
        struct
        {
            unsigned n;
            unsigned v;
        } accepted;
    };
    friend std::ostream &operator<<(std::ostream &os, const I &i)
    {
        switch (i.type)
        {
        case I::REQUEST:
            return os << "<REQUEST>: "
                      << " x=" << i.request.x;
        case I::PREPARE:
            return os << "<PREPARE>: "
                      << " n=" << i.prepare.n;
        case I::PROMISE:
            return os << "<PROMISE>: "
                      << " n=" << i.promise.n
                      << " m=" << i.promise.m
                      << " w=" << i.promise.w;
        case I::ACCEPT:
            return os << "<ACCEPT>: "
                      << " n=" << i.accept.n
                      << " v=" << i.accept.v;
        case I::ACCEPTED:
            return os << "<ACCEPTED>: "
                      << " n=" << i.accepted.n
                      << " v=" << i.accepted.v;
        default:
            return os << "<UNKNOWN>: ";
        }
    }
};

struct O : public sm::std::Queue<S, I, O>::MESSAGES
{
};

class SM : public sm::std::Queue<S, I, O>
{
protected:
    virtual S tf(const I &i, const S &s) override
    {
        std::cout << "[TF][FROM]: " << this << " " << i << " " << s << std::endl;
        S ret = s;
        if (i.type == I::REQUEST)
        {
            if (s.type == S::PROPOSER)
            {
                ret.proposer.n = ret.proposer.n + 1;
                ret.proposer.p = 0;
                ret.proposer.x = i.request.x;
            }
        }
        else if (i.type == I::PREPARE)
        {
            if (s.type == S::ACCEPTOR)
            {
                if (s.acceptor.n < i.prepare.n)
                {
                    ret.acceptor.n = i.prepare.n;
                }
            }
        }
        else if (i.type == I::PROMISE)
        {
            if (s.type == S::PROPOSER)
            {
                if (s.proposer.n == i.promise.n)
                {
                    if (s.proposer.m < i.promise.m)
                    {
                        ret.proposer.m = i.promise.m;
                        ret.proposer.x = i.promise.w;
                        ret.proposer.p = ret.proposer.p + 1;
                    }
                    else
                    {
                        ret.proposer.p = ret.proposer.p + 1;
                    }
                }
            }
        }
        else if (i.type == I::ACCEPT)
        {
            if (s.type == S::ACCEPTOR)
            {
                if (s.acceptor.n <= i.accept.n)
                {
                    ret.acceptor.m = i.accept.n;
                    ret.acceptor.v = i.accept.v;
                }
            }
        }
        else if (i.type == I::ACCEPTED)
        {
            if (s.type == S::LEARNER)
            {
                ret.learner.v = i.accepted.v;
            }
        }
        std::cout << "[TF][TO]: " << this << " " << ret << std::endl;
        return ret;
    }
    virtual O of(const I &i, const S &s) override
    {
        O ret;
        if (i.type == I::REQUEST)
        {
            if (s.type == S::PROPOSER)
            {
                I input;
                input.type = I::PREPARE;
                input.prepare.n = s.proposer.n + 1;
                for (auto &v : GLOBAL::smsa)
                {
                    ret.push_back(MESSAGE(v, input));
                }
            }
        }
        else if (i.type == I::PREPARE)
        {
            if (s.type == S::ACCEPTOR)
            {
                if (s.acceptor.n < i.prepare.n)
                {
                    I input;
                    input.type = I::PROMISE;
                    input.promise.n = i.prepare.n;
                    input.promise.m = s.acceptor.m;
                    input.promise.w = s.acceptor.v;
                    for (auto &v : GLOBAL::smsp)
                    {
                        ret.push_back(MESSAGE(v, input));
                    }
                }
            }
        }
        else if (i.type == I::PROMISE)
        {
            if (s.type == S::PROPOSER)
            {
                if (s.proposer.n == i.promise.n)
                {
                    if (s.proposer.p == GLOBAL::F)
                    {
                        I input;
                        input.type = I::ACCEPT;
                        input.accept.n = s.proposer.n;
                        input.accept.v = s.proposer.x;
                        for (auto &v : GLOBAL::smsa)
                        {
                            ret.push_back(MESSAGE(v, input));
                        }
                    }
                }
            }
        }
        else if (i.type == I::ACCEPT)
        {
            if (s.type == S::ACCEPTOR)
            {
                if (s.acceptor.n <= i.accept.n)
                {
                    I input;
                    input.type = I::ACCEPTED;
                    input.accepted.n = i.accept.n;
                    input.accepted.v = i.accept.v;
                    for (auto &v : GLOBAL::smsp)
                    {
                        ret.push_back(MESSAGE(v, input));
                    }
                    for (auto &v : GLOBAL::smsl)
                    {
                        ret.push_back(MESSAGE(v, input));
                    }
                }
            }
        }
        return ret;
    }
    virtual bool goon(const S &s) override
    {
        return true;
    }
    virtual MESSAGES messages(const O &o) override
    {
        return o;
    }
};

std::array<SM, 1> GLOBAL::smsp;
std::array<SM, 3> GLOBAL::smsa;
std::array<SM, 1> GLOBAL::smsl;

int main()
{

    I i;
    i.type = I::REQUEST;
    i.request.x = 517;
    GLOBAL::smsp[0].send(i);
    std::thread tp([&]() {
        S s;
        s.type = S::PROPOSER;
        s.proposer.n = 1;
        s.proposer.m = 0;
        s.proposer.p = 0;
        s.proposer.x = 0;
        GLOBAL::smsp[0].run(s);
    });
    std::thread ta0([&]() {
        S s;
        s.type = S::ACCEPTOR;
        s.acceptor.n = 0;
        s.acceptor.m = 0;
        s.acceptor.v = 0;
        GLOBAL::smsa[0].run(s);
    });
    std::thread ta1([&]() {
        S s;
        s.type = S::ACCEPTOR;
        s.acceptor.n = 0;
        s.acceptor.m = 0;
        s.acceptor.v = 0;
        GLOBAL::smsa[1].run(s);
    });
    std::thread ta3([&]() {
        S s;
        s.type = S::ACCEPTOR;
        s.acceptor.n = 0;
        s.acceptor.m = 0;
        s.acceptor.v = 0;
        GLOBAL::smsa[2].run(s);
    });
    std::thread tl([&]() {
        S s;
        s.type = S::LEARNER;
        s.learner.v = 0;
        GLOBAL::smsl[0].run(s);
    });
    tp.join();
    return 0;
}