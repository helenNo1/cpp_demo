#include <iostream>
#include <vector>
#include <numeric>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

const int NUM_THREADS = 4;


void calculateSquareSumAsync(const std::vector<int>& numbers, std::promise<int>& promise, int threadId) {
    int partialSum = 0;

    for (std::size_t i = threadId; i < numbers.size(); i += NUM_THREADS) {
        partialSum += numbers[i] * numbers[i];
    }

    // 将计算结果传递给异步任务
    promise.set_value(partialSum);
}
//快速排序


int main() {
    // 准备一组数字
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // 创建异步任务管理器
    boost::asio::io_service ioService;

    // 创建异步任务的承诺和期望
    std::promise<int> promises[NUM_THREADS];
    std::future<int> futures[NUM_THREADS];

    // 启动异步任务
    for (int i = 0; i < NUM_THREADS; ++i) {
        futures[i] = promises[i].get_future();
        ioService.post(boost::bind(calculateSquareSumAsync, std::ref(numbers), boost::ref(promises[i]), i));
    }

    // 等待异步任务完成
    ioService.run();

    // 获取各个异步任务的结果
    std::vector<int> results;
    for (int i = 0; i < NUM_THREADS; ++i) {
        results.push_back(futures[i].get());
    }

    // 计算总和
    int totalSum = std::accumulate(results.begin(), results.end(), 0);

    // 输出结果
    std::cout << "Square sum of numbers: " << totalSum << std::endl;

    return 0;
}
