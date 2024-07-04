#include<functional>
#include<iostream>

int add(int a, int b) {
    return a +b;

}
int main() {
    std::function<int(int,int)> func =add;
    std::cout << "result " << func(3,4) << std::endl;

    std::function<int(int,int)> lambdafunc = [](int x ,int y) {
        return x*y;

    };
    std::cout << lambdafunc(3,4) << std::endl;


    auto bindFunc = std::bind(add, 10, std::placeholders::_1);
    std::cout << bindFunc(5) << std::endl;

}