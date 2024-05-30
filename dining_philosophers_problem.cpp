#include <iostream>
#include <functional>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

class DiningPhilosophers {
    mutex m[5]; // Array of mutexes

public:
    DiningPhilosophers() = default;

    void wantsToEat(int philosopher,
                    function<void()> pickLeftFork,
                    function<void()> pickRightFork,
                    function<void()> eat,
                    function<void()> putLeftFork,
                    function<void()> putRightFork) {
        int leftFork = philosopher;
        int rightFork = (philosopher + 1) % 5;

        // Lock both mutexes using a fixed order to avoid deadlock
        unique_lock<mutex> leftLock(m[min(leftFork, rightFork)]);
        unique_lock<mutex> rightLock(m[max(leftFork, rightFork)]);

        pickLeftFork();
        pickRightFork();
        eat();
        putLeftFork();
        putRightFork();
    }
};

int main() {
    DiningPhilosophers dp;

    vector<thread> philosophers;

    auto pickLeftFork = []() { cout << "Pick left fork" << endl; };
    auto pickRightFork = []() { cout << "Pick right fork" << endl; };
    auto eat = []() { cout << "Eating" << endl; };
    auto putLeftFork = []() { cout << "Put left fork" << endl; };
    auto putRightFork = []() { cout << "Put right fork" << endl; };

    for (int i = 0; i < 5; ++i) {
        philosophers.push_back(thread(&DiningPhilosophers::wantsToEat, &dp, i, pickLeftFork, pickRightFork, eat, putLeftFork, putRightFork));
    }

    for (auto& philosopher : philosophers) {
        philosopher.join();
    }

    return 0;
}
