#include <iostream>
#include <vector>

int main() {
    // 示例 1: auto 推导基本类型
    auto x = 5;            // int
    auto y = 3.14;         // double
    auto flag = true;       // bool
    auto message = "Hello"; // const char*

    std::cout << "x: " << x << ", y: " << y << ", flag: " << flag << ", message: " << message << std::endl;

    // 示例 2: auto 推导容器类型
    std::vector<int> numbers = {1, 2, 3, 4, 5};
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    // 示例 3: auto 推导迭代器类型
    std::string str = "Auto";
    for (auto it = str.begin(); it != str.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << std::endl;

    return 0;
}
