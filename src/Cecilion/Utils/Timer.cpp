#include "Timer.h"

namespace Cecilion {

    Cecilion::Timer::Timer(const char *name) : m_name(name), m_stopped(false) {
        this->m_start_time = std::chrono::high_resolution_clock::now();
    }

    Cecilion::Timer::~Timer() {
        if (!m_stopped) this->Stop();
    }

    void Cecilion::Timer::Stop() {
        auto end_time = std::chrono::high_resolution_clock::now();
        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(this->m_start_time).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(end_time).time_since_epoch().count();
        float duration = (end-start) * 0.001f;
        m_stopped = true;

    }

}