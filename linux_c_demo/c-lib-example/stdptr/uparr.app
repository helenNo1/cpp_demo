#include <memory>
#include <iostream>

int main() {
    const int size = 5;

    // 使用 std::unique_ptr<int[]> 创建指针数组并分配内存
    std::unique_ptr<int[]> arrayPtr = std::make_unique<int[]>(size);

    // 初始化数组
    for (int i = 0; i < size; ++i) {
        arrayPtr[i] = i * 10;
    }

    // 使用数组中的数据
    for (int i = 0; i < size; ++i) {
        std::cout << "Element " << i << ": " << arrayPtr[i] << std::endl;
    }

    // 不需要手动释放内存，当 arrayPtr 超出作用域时，内存会自动释放

    return 0;
}
