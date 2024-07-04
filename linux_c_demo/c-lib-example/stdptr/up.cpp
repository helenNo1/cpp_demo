#include <iostream>
#include <memory>

int main() {
    // 使用 std::unique_ptr 分配一个整数的内存
    std::unique_ptr<int> uniquePtr = std::make_unique<int>(42);

    // 使用 std::unique_ptr 操作指针所指向的值
    std::cout << "Value: " << *uniquePtr << std::endl;

    // std::unique_ptr 会在作用域结束时自动释放内存
    return 0;
}
