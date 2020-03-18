#ifndef CECILION_SEMAPHORE_H
#define CECILION_SEMAPHORE_H

#include <mutex>
#include <condition_variable>

namespace Cecilion {

    /**
    * Simple semaphore class
    */
    class Semaphore {
    public:
        inline explicit Semaphore(unsigned long counter) {this->counter = counter; this->IsActive = true;}
        inline explicit Semaphore() {}
        ~Semaphore();
        void notify();
        void wait();
        void halt();
        void restart();
        void NotifyN(unsigned long counter);
    private:
        std::mutex sem, SemLocked, IsActiveLock;
        std::condition_variable condition;
        unsigned long counter = 0;
        bool IsActive;
    };
}

#endif //CECILION_SEMAPHORE_H
