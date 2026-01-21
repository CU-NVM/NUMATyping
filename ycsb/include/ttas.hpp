#ifndef TTASLOCK_HPP
#define TTASLOCK_HPP

#include <atomic>

class Ttas
{
private:
    std::atomic<bool> flag{false};

public:
    void lock()
    {
        while (flag.load(std::memory_order_acquire) || my_tas() == false) {}
    }

    void unlock() 
    {
        flag.store(false, std::memory_order_release);
    }

    bool my_tas()
    {
        bool expected = false;
        bool desired = true;
        return flag.compare_exchange_strong(expected, desired, std::memory_order_acq_rel);
    }
};

#endif