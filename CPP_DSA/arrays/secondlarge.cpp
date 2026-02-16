#include<iostream>
#include<vector>
#include<algorithm>
using namespace std;

int main()
{
    vector<int> v = {9,8,3,5,2,6,7};
    
    if(v.size() < 2)
    {
        cout << "Not enough elements!" << endl;
        return 1;
    }

    int max = v[0];
    int secMax = INT_MIN;  // Use INT_MIN instead of 0

    for(int i = 1; i < v.size(); i++)
    {
        if(v[i] > max)
        {
            secMax = max;  // Current max becomes second max
            max = v[i];
        }
        else if(v[i] > secMax && v[i] != max)  // Avoid duplicates of max
        {
            secMax = v[i];
        }
    }
    
    if(secMax == INT_MIN)
    {
        cout << "No second largest element found!" << endl;
    }
    else
    {
        cout << "Second Largest: " << secMax << endl;
    }
    
    return 0;
}