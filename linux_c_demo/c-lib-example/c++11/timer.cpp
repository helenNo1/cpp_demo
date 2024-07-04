#include <iostream>
#include <chrono>
#include <thread>
#include <functional>

class Timer
{
public:
    Timer(std::chrono::milliseconds interval, std::function<void()> callback)
        : interval(interval), callback(callback), running(false) {}

    void start()
    {
        running = true;
        timerThread = std::thread([this]()
                                  {
            while (running) {
                std::this_thread::sleep_for(interval);
                if (running) {
                    callback();
                }
            } });
    }

    void stop()
    {
        running = false;
        if (timerThread.joinable())
        {
            timerThread.join();
        }
    }

private:
    std::chrono::milliseconds interval;
    std::function<void()> callback;
    std::thread timerThread;
    bool running;
};

int main()
{
    Timer timer(std::chrono::seconds(1), []()
                { std::cout << "Timer callback executed." << std::endl; });

    // Start the timer
    timer.start();

    // Keep the main thread busy for a while
    std::this_thread::sleep_for(std::chrono::seconds(5));

    // Stop the timer
    timer.stop();

    return 0;
}
