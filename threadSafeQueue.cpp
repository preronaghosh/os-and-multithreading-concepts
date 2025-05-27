/**
 * Blocking Queue : Implement a thread-safe queue with enqueue and dequeue, blocking if empty/full.
*/
#include <iostream>
#include <mutex>
#include <condition_variable>

class BlockingQueue {
    int* arr;
    int front, rear;
    int count, capacity;

    std::mutex mut;
    std::condition_variable full, empty;

public:
    BlockingQueue(int size) : front{-1}, rear{-1}, count{0}, capacity{size} {
        arr = new int[capacity];
    }

    ~BlockingQueue() {
        delete[] arr;
    }

    void push(int value) {
        std::unique_lock<std::mutex> lck(mut);
        // wait for queue to not be full
        full.wait(lck, [this]{ return count < capacity; });
        rear = (rear+1) % capacity;
        arr[rear] = value;
        count++;

        empty.notify_one(); // notify threads blocking on new value in queue
    }
    
    int pop() {
        std::unique_lock<std::mutex> lck(mut);
        // wait for queue to be not empty
        empty.wait(lck, [this]{ return count > 0; });

        int value = arr[front]; 
        front = (front+1) % capacity;
        count--;
        std::cout << "Popped: " << value << std::endl;

        full.notify_one();
        return value;
    }

    int getFront() {
        std::lock_guard<std::mutex> lck(mut);
        if (isEmpty()) { return -1; }
        return arr[front];
    }

    int getRear() { 
        std::lock_guard<std::mutex> lck(mut);
        if (isEmpty()) { return -1; }
        return arr[rear];
    }

    bool isEmpty() {
        std::lock_guard<std::mutex> lck(mut);
        return count == 0;
    }

    bool isFull() {
        std::lock_guard<std::mutex> lck(mut);
        return count == capacity;
    }
};