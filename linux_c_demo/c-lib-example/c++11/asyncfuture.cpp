#include <iostream>
#include <future>

// Function to calculate the sum of numbers from start to end (inclusive)
int sum(int start, int end)
{
    int result = 0;
    for (int i = start; i <= end; ++i)
    {
        result += i;
    }
    return result;
}

int main()
{
    // Asynchronously calculate the sum of numbers from 1 to 100
    std::future<int> futureResult = std::async(sum, 1, 100);

    // Do some other work while the sum is being calculated asynchronously

    // Obtain the result when needed
    int result = futureResult.get();

    // Print the result
    std::cout << "Sum of numbers from 1 to 100: " << result << std::endl;

    return 0;
}
