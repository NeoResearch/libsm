#pragma once
#include <mutex>
#include <condition_variable>
#include <deque>

namespace sm
{
namespace std
{
template <typename S, typename I, typename O>
class Queue : public sm::SM<S, I, O>
{
public:
    using MESSAGE = ::std::pair<Queue &, I>;
    using MESSAGES = ::std::vector<MESSAGE>;
    Queue() : SM<S, I, O>([&] { return recv(); }, [&](const O &o) { for (auto&v:messages(o)) v.first.send(v.second); }) {}
    void send(I const &value)
    {
        {
            ::std::unique_lock<::std::mutex> lock(this->d_mutex);
            d_queue.push_front(value);
        }
        this->d_condition.notify_one();
    }
    I recv()
    {
        ::std::unique_lock<::std::mutex> lock(this->d_mutex);
        this->d_condition.wait(lock, [=] { return !this->d_queue.empty(); });
        I rc(::std::move(this->d_queue.back()));
        this->d_queue.pop_back();
        return rc;
    }

protected:
    virtual MESSAGES messages(const O &o)
    {
        return MESSAGES();
    }

private:
    ::std::mutex d_mutex;
    ::std::condition_variable d_condition;
    ::std::deque<I> d_queue;
};
} // namespace std
} // namespace sm