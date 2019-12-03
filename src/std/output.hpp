#pragma once
#include <vector>
#include "event.hpp"
namespace sm
{
namespace std
{
template <typename Input>
class Output
{
public:
    Output() {}
    void push(::std::unique_ptr<Event<Input, Output>> &&ptr)
    {
        data.push_back(::std::move(ptr));
    }
    virtual const ::std::vector<::std::unique_ptr<Event<Input, Output>>> &events() const
    {
        return data;
    }

private:
    ::std::vector<::std::unique_ptr<Event<Input, Output>>> data;
};
} // namespace std
} // namespace sm