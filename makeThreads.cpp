#include <iostream>
#include <thread>

class Base {
public:
    void run(int& x) {
        while (x-- > 0) {
            std::cout << x << std::endl;
        }
    }
};

int main() {
    int val = 10;

    Base b;
    std::thread t1(&Base::run, &b, std::ref(val));
    t1.join();
    return 0;
}