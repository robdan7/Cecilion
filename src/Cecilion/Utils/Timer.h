#pragma once
#include <chrono>
namespace Cecilion {
    template<typename Func>
    class Timer {
    public:
        Timer(const char* name);
        ~Timer();
        void Stop();
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start_time;
        const char* m_name;
        bool m_stopped;
    };
}