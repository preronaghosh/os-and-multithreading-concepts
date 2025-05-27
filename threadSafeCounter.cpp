#include <mutex>
#include <atomic>

class Counter {
    int counter = 0;
    mutable std::mutex counterMutex;
    
public:
    void increment() {
        std::lock_guard<std::mutex> lck(counterMutex);
        counter++;
    } 

    void decrement() {
        std::lock_guard<std::mutex> lck(counterMutex);
        counter--;
        if (counter < 0) counter = 0;
    }
    
    int getCount() const {
        std::lock_guard<std::mutex> lck(counterMutex);
        return counter;
    }
};

// ------------------------------------------------------------------------------------

class AtomicCounter {
    std::atomic<int> counter{0};
public:
    void increment() { counter.fetch_add(1); }
    void decrement() { 
        int val = counter.load();
        while (val > 0 && !counter.compare_exchange_weak(val, val - 1)) {
            // retry
        }
    }
    int getCount() const { return counter.load(); }
};
