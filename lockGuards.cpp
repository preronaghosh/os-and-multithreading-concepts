#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex m;
int balance = 0;

void addMoney(int x) {
    std::unique_lock<std::mutex> lock(m);
    std::cout << "Adding " << x << " in thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    balance+= x;
}

int main() {
    std::thread t1(addMoney, 10);
    std::thread t2(addMoney, 10);
    std::thread t3(addMoney, 40);

    t1.join();
    t2.join();
    t3.join();

    std::cout << "Final amount = " << balance << std::endl;

    return 0;
}