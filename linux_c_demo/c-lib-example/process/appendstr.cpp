#include <cstring>
#include <iostream>

int main()
{
    char str1[10] = "hello";
    char str2[5] = "good";

    // 找到str1的末尾
    char *end = str1;
    while (*end != '\0')
    {
        end++;
    }

    // 将要追加的字符串部分拷贝到str1的末尾
    strncpy(end, str2, 2);

    // 设置下一个位置为'\0'
    end += 2;
    *end = '\0';

    // 输出结果
    std::cout << str1 << std::endl; // 打印出"hellogo"

    return 0;
}