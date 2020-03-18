#include "Semaphore.h"
#include <thread>

void Cecilion::Semaphore::notify() {
    std::lock_guard<decltype(this->sem)> lock(this->sem);
    counter ++;
    condition.notify_one();
}

void Cecilion::Semaphore::wait() {
    std::unique_lock<decltype(this->sem)> lock(this->sem);
    while(!this->counter) {
        condition.wait(lock);
    }
    this->counter--;
    this->IsActiveLock.lock();
    if (!this->IsActive) {

        this->SemLocked.lock();
    }
    this->IsActiveLock.unlock();
}

/**
 * Completely halt this semaphore. It will not be possible to lock it again until the semaphore is restarted.
 *
 * TODO Not tested.
 */
void Cecilion::Semaphore::halt() {
    //std::unique_lock<decltype(this->sem)> lock(this->IsActive);
    if (!this->IsActive) {
        return;
    }
    this->IsActiveLock.lock();
    this->SemLocked.lock();
    this->IsActive = false;
    this->IsActiveLock.unlock();
}

/**
 * Restart the semaphore if it is currently locked. Nothing will happen if the semaphore is currently active.
 * TODO Not tested.
 */
void Cecilion::Semaphore::restart() {
    if (this->IsActive) {
        return;
    }
    if (!this->SemLocked.try_lock()) {
        this->IsActive = true;
        this->SemLocked.unlock();
    } else {
        this->IsActive = true;
        this->SemLocked.unlock();
    }
}

/**
 * TODO Not tested.
 * @param counter
 */
void Cecilion::Semaphore::NotifyN(unsigned long counter) {
    std::lock_guard<decltype(this->sem)> lock(this->sem);
    counter += counter;
    for (int i = 0; i < counter; i++) {
        condition.notify_one();
    }
}



//bool Cecilion::Semaphore::TryWait() {
//    std::lock_guard<decltype(this->sem)> lock(this->sem);
//    if (!this->counter || !this->IsActive) {
//        return false;
//    }
//    this->counter--;
//    return true;
//}
