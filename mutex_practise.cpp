// Example program to demonstrate usage of mutex class in C++

#include <mutex>
#include <thread>
#include <iostream>

using namespace std::literals;

// Global mutex for all threads
std::mutex mutex;

void task1() {
    std::cout << "Task 1 Locking the mutex" << std::endl;
    mutex.lock();

    std::cout << "Task 1 has locked the mutex" << std::endl;
    std::this_thread::sleep_for(500ms);

    std::cout << "Task 1 unlocking the mutex" << std::endl;
    mutex.unlock();
}

void task2() {
    std::cout << "Task 2 trying to lock the mutex" << std::endl;

    while(!mutex.try_lock()) {
        std::cout << "Task 2 could not lock the mutex" << std::endl;
        std::this_thread::sleep_for(100ms);
    }

    std::cout << "Task 2 has locked the mutex" << std::endl;
    mutex.unlock();
    std::cout << "Task 2 has unlocked the mutex" << std::endl;
}


int main() {
    std::thread th1(task1);
    std::thread th2(task2);

    th1.join();
    th2.join();
}