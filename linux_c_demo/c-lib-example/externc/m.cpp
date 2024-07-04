#include <iostream>

#ifdef __cplusplus
#define MY_FEATURE_SUPPORTED

// 在 C++ 编译器中使用 C++ 特性
template <typename T>
void my_feature(const T& value) {
    std::cout << "C++ Feature: " << value << std::endl;
}

#else

// 在其他编译器中使用普通的 C 特性
#define MY_FEATURE_SUPPORTED

void my_feature(const char* value) {
    printf("C Feature: %s\n", value);
}

#endif

int main() {
#ifdef MY_FEATURE_SUPPORTED
    my_feature("Hello, World!");
#endif
    return 0;
}
