#pragma once
#include "def.hpp"
#include "state.hpp"
#include "input.hpp"
#include "output.hpp"
class SM : public sm::std::Queue<S, I, O>
{
protected:
    virtual S tf(const I &i, const S &s) override
    {
        // TODO
        return s;
    }
    virtual O of(const I &i, const S &s) override
    {
        O ret;
        // TODO
        return ret;
    }
    virtual bool goon(const S &s) override
    {
        return true;
    }
    virtual MESSAGES messages(const O &o) override
    {
        MESSAGES ret;
        for (auto &v : o)
        {
            if (v.type == Out::MESSAGE)
            {
                ret.push_back(v.message);
            }
        }
        return ret;
    }
};