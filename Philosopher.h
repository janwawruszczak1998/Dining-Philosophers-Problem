//
// Created by jan on 25.03.2020.
//

#ifndef DINING_PHILOSOPHERS_PHILOSOPHER_H
#define DINING_PHILOSOPHERS_PHILOSOPHER_H

#include <vector>
#include <memory>
#include "Fork.h"
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <atomic>
#include <ncurses.h>


class Philosopher{
public:

    Philosopher(int id, int num_of_forks, std::vector<std::unique_ptr<Fork> >& f, WINDOW *phi_window)
    : phi_id(id), num_of_forks(num_of_forks),
    forks(f), phi_window(phi_window)
    {
        forks.resize(num_of_forks);
        this->run();
    }

    void think(){
        for (int i = 15; i >= 0; i--)
        {
            {
                std::lock_guard<std::mutex> writing_lock(writing_mutex);
                mvwprintw(phi_window, 2, 1, "STATE: THINKING %2d", i);
                wrefresh(phi_window);


            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 50)));
        }
    }

    void being_philosopher(){
        // filozof moze filozofowac z wieksza szansa niz jesc
        // zapobiega to zaglodzeniu, bo zmniejsza szanse na konkurencje o zasob
        while(end_flag == false){
            int x = rand() % 3;
            if(x == 0){
                eat();
            }
            else{
                think();
            }
        }
    }

    void eat(){
        // ostatni filozof najpierw bierze prawy widelec, w przeciwienstwie do pozostalych
        // chroni to przez zakleszczeniem
        if(phi_id == num_of_forks - 1){
            if(forks[(phi_id + 1) % num_of_forks]->fork_mutex.try_lock()){
                if(forks[phi_id]->fork_mutex.try_lock()){
                    if(forks[phi_id]->is_occupied == false && forks[(phi_id + 1) % num_of_forks]->is_occupied == false){
                        std::lock_guard<std::mutex> left_fork(forks[phi_id]->fork_mutex, std::adopt_lock);
                        std::lock_guard<std::mutex> right_fork(forks[(phi_id + 1) % num_of_forks]->fork_mutex, std::adopt_lock);
                        forks[phi_id]->is_occupied = true;
                        forks[(phi_id + 1) % num_of_forks]->is_occupied = true;
                        forks[phi_id]->show_status();
                        forks[(phi_id + 1) % num_of_forks]->show_status();
                        for (int i = 15; i >= 0; i--)
                        {
                            {
                                std::lock_guard<std::mutex> writing_lock(writing_mutex);
                                mvwprintw(phi_window, 2, 1, "STATE: EATING %2d", i);
                                wrefresh(phi_window);
                                  
                            }
                            std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 50)));
                        }
                        //widelce tez sa odkladane w odwrotnej kolejnosci
                        forks[(phi_id + 1) % num_of_forks]->is_occupied = false;
                        forks[phi_id]->is_occupied = false;
                        forks[phi_id]->show_status();
                        forks[(phi_id + 1) % num_of_forks]->show_status();

                        forks[(phi_id + 1) % num_of_forks]->fork_mutex.unlock();
                        forks[phi_id]->fork_mutex.unlock();
                        return;
                    }
                    else{
                        forks[(phi_id + 1) % num_of_forks]->fork_mutex.unlock();
                        forks[phi_id]->fork_mutex.unlock();
                        return;
                    }
                }
                else{
                    // jesli wzielismy tylko jeden widelec a nie udalo sie wziac drugiego, to odkladamy widelec
                    // i usypiamy watek na chwile
                    forks[(phi_id + 1) % num_of_forks]->fork_mutex.unlock();
                    for (int i = 2; i >= 0; i--)
                    {
                        {
                            std::lock_guard<std::mutex> writing_lock(writing_mutex);
                            mvwprintw(phi_window, 2, 1, "STATE: HUNGRY %2d", i);
                            wrefresh(phi_window);
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 50)));
                    }
                    return;
                }
            }
            else {
                for (int i = 2; i >= 0; i--)
                {
                    {
                        std::lock_guard<std::mutex> writing_lock(writing_mutex);
                        mvwprintw(phi_window, 2, 1, "STATE: HUNGRY %2d", i);
                        wrefresh(phi_window);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 50)));
                }
                return;                return;
            }
        }



        // pozostali filozofowie biora najpierw lewy widelec
        if(forks[phi_id]->fork_mutex.try_lock()){
            if(forks[(phi_id + 1) % num_of_forks]->fork_mutex.try_lock()){
                if(forks[phi_id]->is_occupied == false && forks[(phi_id + 1) % num_of_forks]->is_occupied == false){
                    std::lock_guard<std::mutex> left_fork(forks[phi_id]->fork_mutex, std::adopt_lock);
                    std::lock_guard<std::mutex> right_fork(forks[(phi_id + 1) % num_of_forks]->fork_mutex, std::adopt_lock);
                    forks[phi_id]->is_occupied = true;
                    forks[(phi_id + 1) % num_of_forks]->is_occupied = true;
                    forks[phi_id]->show_status();
                    forks[(phi_id + 1) % num_of_forks]->show_status();
                    for (int i = 15; i >= 0; i--)
                    {
                        {
                            std::lock_guard<std::mutex> writing_lock(writing_mutex);
                            mvwprintw(phi_window, 2, 1, "STATE: EATING %2d", i);
                            wrefresh(phi_window);
                              
                        }
                        std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 50)));
                    }
                    forks[phi_id]->is_occupied = false;
                    forks[(phi_id + 1) % num_of_forks]->is_occupied = false;
                    forks[phi_id]->show_status();
                    forks[(phi_id + 1) % num_of_forks]->show_status();
                    forks[phi_id]->fork_mutex.unlock();
                    forks[(phi_id + 1) % num_of_forks]->fork_mutex.unlock();
                    return;
                }
                else{
                    forks[phi_id]->fork_mutex.unlock();
                    forks[(phi_id + 1) % num_of_forks]->fork_mutex.unlock();
                    return;
                }
            }
            else{
                forks[phi_id]->fork_mutex.unlock();
                for (int i = 2; i >= 0; i--)
                {
                    {
                        std::lock_guard<std::mutex> writing_lock(writing_mutex);
                        mvwprintw(phi_window, 2, 1, "STATE: HUNGRY %2d", i);
                        wrefresh(phi_window);
                    }
                    std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 50)));
                }
                return;
            }
        }
        else {
            for (int i = 2; i >= 0; i--)
            {
                {
                    std::lock_guard<std::mutex> writing_lock(writing_mutex);
                    mvwprintw(phi_window, 2, 1, "STATE: HUNGRY %2d", i);
                    wrefresh(phi_window);
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(500 + (rand() % 50)));
            }
            return;
        }
    }

    void run(){
        phi_thread = std::thread{ [this](){this->being_philosopher();} };
        //phi_thread.join();
    }

//private:
    std::mutex writing_mutex;
    int phi_id, num_of_forks;
    static std::atomic_bool end_flag;
    std::vector<std::unique_ptr<Fork> >& forks;
    std::thread phi_thread;
    WINDOW *phi_window;
};

#endif //DINING_PHILOSOPHERS_PHILOSOPHER_H
