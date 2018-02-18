#ifndef __SPINLOCK_HPP__
#define __SPINLOCK_HPP__
#include <atomic>

///Spinlock class, used to lock for short periods
class Spinlock
{
public:
    void lock()
    {
        while(lck.test_and_set(std::memory_order_acquire))
        {}
    }
    
    void unlock()
    {
        lck.clear(std::memory_order_release);
    }
    
private:
    std::atomic_flag lck = ATOMIC_FLAG_INIT;
};

#endif
