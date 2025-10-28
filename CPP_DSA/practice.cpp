#include <iostream>
#include <vector>
using namespace std;
int main()
{
    // Create a vector of integers
    vector<int> numbers = {1, 2, 3, 4, 5};

    // Print the original vector
    cout << "Original vector: ";
    for (const int &num : numbers)
    {
        cout << num << " ";
    }
    cout << endl;

    // Modify the vector by adding 10 to each element
    for (int &num : numbers)
    {
        num += 10;
    }

    // Print the modified vector
    cout << "Modified vector: ";
    for (const int &num : numbers)
    {
        cout << num << " ";
    }
    cout << endl;

    return 0;
}