#include <iostream>
#include <vector>
#include "Fork.h"
#include "Philosopher.h"

int NUM_OF_PHILOSOPHERS = 5; // zakladamy tyle samo widelcow

int main() {
    std::vector<std::unique_ptr<Philosopher>> philosophers;
    std::vector<std::unique_ptr<Fork>> forks;
    for(int i = 0; i < NUM_OF_PHILOSOPHERS; ++i){
        std::unique_ptr<Fork> fork(new Fork);
        forks.push_back(std::move(fork));
    }

    for(int i = 0; i < NUM_OF_PHILOSOPHERS; ++i){
        std::unique_ptr<Philosopher> philosopher(new Philosopher(i, NUM_OF_PHILOSOPHERS, forks));
        philosophers.push_back(std::move(philosopher));
    }
    for(int i = 0; i < NUM_OF_PHILOSOPHERS; ++i) {
        philosophers[i]->phi_thread.join();
    }

    std::cout << "Hello, World!" << std::endl;
    return 0;
}
