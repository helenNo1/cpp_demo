#include <iostream>
#include <memory>

int main() {
    // 使用 std::shared_ptr 分配一个整数的内存
    std::shared_ptr<int> sharedPtr = std::make_shared<int>(42);

    // 使用 std::shared_ptr 操作指针所指向的值
    std::cout << "Value: " << *sharedPtr << std::endl;

    // 创建新的 sharedPtr2，共享同一块内存
    std::shared_ptr<int> sharedPtr2 = sharedPtr;

    // 引用计数器会增加
    std::cout << "Reference count: " << sharedPtr.use_count() << std::endl;

    // std::shared_ptr 会在最后一个引用计数为零时释放内存
    return 0;
}
