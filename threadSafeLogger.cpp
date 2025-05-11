/**
Thread-safe Logger:

- Multiple threads log messages to a shared queue.
- A background thread flushes the queue to a file.
- Use condition_variable to wake up logger only when data is available.

 */

#include <thread>
#include <iostream>
#include <string> 
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <fstream>

std::queue<std::string> messageQueue;
std::mutex queueMutex;
std::condition_variable cv;
std::atomic<bool> abortFlag{false};


void writeLogToFile(const std::string& log) {
    std::ofstream logFile("logs.txt", std::ios::app);
    if (!logFile.is_open()) {
        std::cout << "Failed to open file." << std::endl;
        return;
    }
    logFile << log << std::endl;
    logFile.close();
}

void addLogToQueue(const std::string& message) {
    std::lock_guard<std::mutex> lck(queueMutex);
    messageQueue.push(message);
    cv.notify_all();
}

void consumeQueueMessage() {
    while (!abortFlag.load()) {
        std::unique_lock<std::mutex> ul(queueMutex);
        cv.wait(ul, []{ return !messageQueue.empty() || abortFlag.load(); });
        if (!messageQueue.empty()) {
            std::string newLog = messageQueue.front();
            messageQueue.pop();
            std::cout << "Consumed log: " << newLog << std::endl;
            writeLogToFile(newLog);
        }
    }
}


int main() {
    std::thread t4(consumeQueueMessage);

    std::thread t1(addLogToQueue, "Hello World");
    std::thread t2(addLogToQueue, "Hello World 2");
    std::thread t3(addLogToQueue, "Hello World 3");
    std::thread t5(addLogToQueue, "this is a new log");

    t1.join();
    t2.join();
    t3.join();
    t5.join();

    std::cout << "Aborting.." << std::endl;
    abortFlag.store(true);
    cv.notify_all();

    t4.join();
    std::cout << "All threads finished.." << std::endl;

    return 0;
}