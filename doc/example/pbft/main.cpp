#include "sm.hpp"
#include <thread>
int main()
{
    std::array<SM, GLOBAL::N> nodes;
    std::array<std::thread, GLOBAL::N> threads = {
        std::thread([&] {
            S s = {};
            s.index = 0;
            nodes[0].run(s);
        }),
        std::thread([&] {
            S s = {};
            s.index = 1;
            nodes[1].run(s);
        }),
        std::thread([&] {
            S s = {};
            s.index = 2;
            nodes[2].run(s);
        }),
        std::thread([&] {
            S s = {};
            s.index = 3;
            nodes[3].run(s);
        }),
    };
    I i;
    i.type = I::REQUEST;
    i.request.o = i.request.INC;
    i.request.t = 1;
    i.request.c = 0;
    nodes[0].send(i);
    for (auto &v : threads)
    {
        v.join();
    }
    return 0;
}