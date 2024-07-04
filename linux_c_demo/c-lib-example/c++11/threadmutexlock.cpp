#include <iostream>
#include <mutex>
#include <thread>

std::mutex counterMutex;
int sharedCounter = 0;

void incrementCounter(int id) {
    for (int i = 0; i < 5; ++i) {
        std::unique_lock<std::mutex> lock(counterMutex);

        // Critical section: increment the shared counter
        ++sharedCounter;

        // Print thread ID and updated counter value
        std::cout << "Thread ID " << id << ": Counter = " << sharedCounter << std::endl;

        // lock is automatically released when it goes out of scope
    }
}

int main() {
    const int numThreads = 3;
    std::thread threads[numThreads];

    // Launch threads
    for (int i = 0; i < numThreads; ++i) {
        threads[i] = std::thread(incrementCounter, i + 1);
    }

    // Join threads
    for (int i = 0; i < numThreads; ++i) {
        threads[i].join();
    }

    return 0;
}
