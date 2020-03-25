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

class Philosopher{
public:

    Philosopher(int id, int num_of_forks, std::vector<std::unique_ptr<Fork> >& f)
    : phi_id(id), end_flag(false), num_of_forks(num_of_forks),
    forks(f)
    {
        forks.resize(num_of_forks);
        this->run();
    }

    void think(){
        std::cout << phi_id << " is thinking..." << std::endl;
        int x = 2000;
        std::this_thread::sleep_for(std::chrono::milliseconds(x));
    }

    void being_philosopher(){
        while(end_flag == false){
            int x = rand() % 5;
            if(x == 0){
                eat();
            }
            else{
                think();
            }
        }
    }

    void eat(){
        std::cout << "..............." << phi_id << " is trying to eat" << std::endl;
        if(forks[phi_id]->fork_mutex.try_lock()){
            std::cout << phi_id << " locked one fork" << std::endl;
            if(forks[(phi_id + 1) % num_of_forks]->fork_mutex.try_lock()){
                std::cout << phi_id << " locked two forks" << std::endl;
                if(forks[phi_id]->is_occupied == false && forks[(phi_id + 1) % num_of_forks]->is_occupied == false){
                    std::lock_guard<std::mutex> left_fork(forks[phi_id]->fork_mutex, std::adopt_lock);
                    std::lock_guard<std::mutex> right_fork(forks[(phi_id + 1) % num_of_forks]->fork_mutex, std::adopt_lock);
                    std::cout << phi_id << " is eating................................." << std::endl;
                    forks[phi_id]->is_occupied = true;
                    forks[(phi_id + 1) % num_of_forks]->is_occupied = true;
                    std::this_thread::sleep_for(std::chrono::milliseconds(1500 + rand() % 1000));
                    forks[phi_id]->is_occupied = false;
                    forks[(phi_id + 1) % num_of_forks]->is_occupied = false;
                    forks[phi_id]->fork_mutex.unlock();
                    forks[(phi_id + 1) % num_of_forks]->fork_mutex.unlock();
                    return;
                }
                else{
                    std::cout << "weird, why?" << phi_id << std::endl;
                    forks[phi_id]->fork_mutex.unlock();
                    forks[(phi_id + 1) % num_of_forks]->fork_mutex.unlock();
                    return;
                }
            }
            else{
                std::cout << "no two forks for " << phi_id << std::endl;
                forks[phi_id]->fork_mutex.unlock();
                return;
            }
        }
        else {
            std::cout << phi_id << " is hungry" << std::endl;
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            return;
        }
    }

    void run(){
        phi_thread = std::thread{ [this](){this->being_philosopher();} };
        //phi_thread.join();
    }

//private:

    int phi_id, num_of_forks;
    std::atomic_bool end_flag;
    std::vector<std::unique_ptr<Fork> >& forks;
    std::thread phi_thread;
};

#endif //DINING_PHILOSOPHERS_PHILOSOPHER_H
