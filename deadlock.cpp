#include <iostream>
#include <thread>
#include <mutex>

std::mutex m1;
std::mutex m2;

void taskA() {
    std::lock(m1, m2);

    std::lock_guard<std::mutex> lock1(m1, std::adopt_lock);
    std::cout << "Started task A.." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work of task A
    std::lock_guard<std::mutex> lock2(m2, std::adopt_lock);
}

void taskB() {
    std::lock(m1, m2);

    std::lock_guard<std::mutex> lock1(m2, std::adopt_lock);
    std::cout << "Started task B.." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(2)); // Simulate work of task B
    std::lock_guard<std::mutex> lock2(m1, std::adopt_lock);
}


int main() {

    std::thread t1(taskA);
    std::thread t2(taskB);

    t1.join();
    t2.join();

    return 0;
}