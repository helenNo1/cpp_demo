#include <iostream>

int main() {
    int x = 42;

    // 使用 const auto& 声明对 x 的常量引用
    const auto& refToX = x;

    // 通过引用可以读取 x 的值，但不能修改 x 的值
    std::cout << "Value of x: " << refToX << std::endl;

    // 编译器会阻止修改通过常量引用的对象的尝试
    // refToX = 10;  // 这行代码会导致编译错误

    return 0;
}
