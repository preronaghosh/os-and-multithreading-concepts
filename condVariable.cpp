#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
#include <condition_variable>

int balance = 0;
std::mutex m;
std::condition_variable cv;

void addMoney(int x) {
    std::lock_guard<std::mutex> lock(m);
    balance += x;
    std::cout << "added money: " << x << " ; Current Balance = " << balance << std::endl;
    cv.notify_one();
}

void withdrawMoney(int x) {
    std::unique_lock<std::mutex> lock(m);
    cv.wait_for(lock, std::chrono::seconds(1), []{ return balance != 0 ? true : false; });
    if (balance >= x) {
        balance -= x;
        std::cout << "Withdrew money: " << x << " ; Current Balance = " << balance << std::endl;
    } else {
        std::cout << "Cannot withdraw money" <<  " ; Current Balance = " << balance << std::endl;
    }
}

int main() {
    std::thread t1(addMoney, 500);
    std::thread t2(withdrawMoney, 500);
    std::thread t3(addMoney, 200);
    std::thread t4(withdrawMoney, 10);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
