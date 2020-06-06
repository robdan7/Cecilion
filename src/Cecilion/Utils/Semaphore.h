#pragma once
#include <mutex>
#include <condition_variable>

namespace Cecilion {

    /**
    * Simple semaphore class
    */
    class Semaphore {
    public:
        inline explicit Semaphore(unsigned long counter) {this->counter = counter;}
        inline explicit Semaphore() {}
        ~Semaphore();
        void notify();
        void wait();
//        void halt();
//        void restart();
        void NotifyN(unsigned long counter);
        bool try_wait();
    private:
        std::mutex sem;
        std::condition_variable condition;
        unsigned long counter = 0;
    };
}

