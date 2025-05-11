#include <iostream>
#include <thread>
#include <future>

void findOddSum(std::promise<int>&& promise, const int& start, const int& end) {
    int oddSum = 0;
    for (int i=start; i<=end; i++) {
        if (i % 2 != 0) {
            oddSum += i;
        }
    }
    promise.set_value(oddSum);
}

int main() {
    std::promise<int> oddSumPromise;
    std::future<int> oddSumFut = oddSumPromise.get_future();

    std::thread t1(findOddSum, std::move(oddSumPromise), 1, 5);

    std::cout << "Waiting for future result" << std::endl;
    auto result = oddSumFut.get();
    std::cout << "Received oddSum = " << result << "\n";

    t1.join();
    return 0;
}
