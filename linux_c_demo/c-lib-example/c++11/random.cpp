#include <time.h>
#include <iostream>
#include <random>

using namespace std;

int main(int argc, char const *argv[])
{

    std::default_random_engine random(time(nullptr));
    std::uniform_int_distribution<int> int_dis(0, 100);
    std::uniform_real_distribution<float> real_dis(0.0, 1.0);

    for (int i = 0; i < 10; ++i)
    {
        cout << int_dis(random) << ' ';
    }
    cout << endl;

    for (int i = 0; i < 10; ++i)
    {
        cout << real_dis(random) << ' ';
    }
    cout << endl;

    return 0;
}
