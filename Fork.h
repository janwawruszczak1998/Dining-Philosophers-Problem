#ifndef DINING_PHILOSOPHERS_FORK_H
#define DINING_PHILOSOPHERS_FORK_H

#include <mutex>
#include <atomic>
#include <thread>

class Fork{
public:
    Fork(WINDOW *fork_window)
    : is_occupied(false), fork_window(fork_window){
    }

    void show_status(){
        if(is_occupied){
            mvwprintw(fork_window, 2, 1, "OCCUPIED");
            wrefresh(fork_window);
        }
        else{
            mvwprintw(fork_window, 2, 1, "FREE    ");
            wrefresh(fork_window);
        }

    }

    std::atomic<bool> is_occupied;
    std::mutex fork_mutex;
    WINDOW *fork_window;
};

#endif // DINING_PHILOSOPHERS_FORK_H