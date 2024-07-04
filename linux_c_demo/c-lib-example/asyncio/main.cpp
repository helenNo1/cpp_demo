#include <iostream>
#include <future>

int myFunction() {
    // Perform some asynchronous operation
    return 42;
}

int main() {
    std::future<int> result = std::async(myFunction);

    // Do other work while myFunction is executing asynchronously

    int finalResult = result.get();  // Wait for the result
    std::cout << "Result: " << finalResult << std::endl;

    return 0;
}
