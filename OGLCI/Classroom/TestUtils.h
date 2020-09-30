#pragma once
#include <chrono>

template<typename TimeT = std::milli>
struct measure
{
    template<typename F, typename ...Args>
    static std::chrono::duration<float, TimeT> duration(F&& func, Args&&... args)
    {
        std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
        std::invoke(std::forward<F>(func), std::forward<Args>(args)...);
        return std::chrono::duration<float, TimeT>(std::chrono::steady_clock::now() - start);
    }
};

