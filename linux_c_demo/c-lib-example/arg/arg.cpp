#include <stdio.h>
#include <stdarg.h>

void example_function(int num, ...)
{
    va_list args;
    va_start(args, num); // 初始化 va_list，传递最后一个已知参数的前一个参数

    for (int i = 0; i < num; ++i)
    {
        int arg = va_arg(args, int); // 获取下一个参数
        printf("%d ", arg);
    }

    va_end(args); // 清理 va_list
}

int main()
{
    example_function(3, 1, 2, 3);
    return 0;
}
