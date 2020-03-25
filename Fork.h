#ifndef DINING_PHILOSOPHERS_FORK_H
#define DINING_PHILOSOPHERS_FORK_H

#include <mutex>
#include <atomic>

class Fork{
public:
    Fork()
    : is_occupied(false){
    }

    std::atomic<bool> is_occupied;
    std::mutex fork_mutex;
};

#endif // DINING_PHILOSOPHERS_FORK_H