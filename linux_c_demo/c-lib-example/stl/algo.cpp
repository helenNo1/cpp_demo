#include <algorithm>
#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

int main()
{

    int arr[] = {5, 3, 2, 1, 4};
    int n = sizeof(arr) / sizeof(arr[0]);
    vector<int> vect(arr, arr + n);
    cout << "vector is:";
    for (int i = 0; i < n; i++)
    {
        cout << vect[i] << " ";
    }
    sort(vect.begin(), vect.end());
    cout << "\n vector after sort is:";
    for (int i = 0; i < n; i++)
    {
        cout << vect[i] << " ";
    }

    sort(vect.begin(), vect.end(), greater<int>());
    cout << "vector after sort in descend is:";
    for (int i = 0; i < n; i++)
    {
        cout << vect[i] << " ";
    }
    reverse(vect.begin(), vect.end());
    cout << "vector after reverse in descend is:";
    for (int i = 0; i < n; i++)
    {
        cout << vect[i] << " ";
    }

    cout << "max is:";
    cout << *max_element(vect.begin(), vect.end());
    cout << "min is:";
    cout << *min_element(vect.begin(), vect.end());

    cout << "sum is:";
    cout << accumulate(vect.begin(), vect.end(), 0);
    return 0;
}