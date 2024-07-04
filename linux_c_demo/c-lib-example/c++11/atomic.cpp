#include <iostream>
#include <atomic>
#include <thread>

std::atomic<int> sharedCounter(0);

void incrementCounter(int id) {
    for (int i = 0; i < 5; ++i) {
        // Atomic increment operation
        ++sharedCounter;

        // Print thread ID and updated counter value
        std::cout << "Thread ID " << id << ": Counter = " << sharedCounter << std::endl;
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
