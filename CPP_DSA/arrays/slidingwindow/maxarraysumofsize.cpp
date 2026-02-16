#include<iostream>
#include<vector>
using namespace std;

int main()
{
    vector<int> arr = {1, 2, 6, 2, 4, 1,7};
    int k = 3;
    int n = arr.size();
    
    // Sliding Window Approach - O(n)
    int maxSum = 0;
    
    // Calculate sum of first k elements
    for(int i = 0; i < k; i++)
    {
        maxSum += arr[i];
    }
    
    int currentSum = maxSum;
    
    // Slide the window and track maximum
    for(int i = k; i < n; i++)
    {
        currentSum += arr[i] - arr[i - k];
        maxSum = max(maxSum, currentSum);
    }
    
    cout << "Maximum Subarray Sum of Size " << k << ": " << maxSum << endl;
    return 0;
}