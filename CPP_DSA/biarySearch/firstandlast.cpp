#include<iostream>
#include<vector>

using namespace std;

int findFirst(vector<int> arr, int target)
{
    int low = 0, high = arr.size() - 1;
    int first = -1;
    
    while(low <= high)
    {
        int mid = (low + high) / 2;
        if(arr[mid] == target)
        {
            first = mid;
            high = mid - 1;  // Continue searching in left half
        }
        else if(arr[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return first;
}

int findLast(vector<int> arr, int target)
{
    int low = 0, high = arr.size() - 1;
    int last = -1;
    
    while(low <= high)
    {
        int mid = (low + high) / 2;
        if(arr[mid] == target)
        {
            last = mid;
            low = mid + 1;  // Continue searching in right half
        }
        else if(arr[mid] < target)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    return last;
}

vector<int>firstAndLast(vector<int> arr, int target)
{
    int first = findFirst(arr, target);
    int last = findLast(arr, target);
    
    return {first, last};
}
int main()
{
    vector<int> arr = {5,7,7,8,8,10,12};
    int target = 8;

    vector<int> result = firstAndLast(arr, target);
    cout << "First occurrence: " << result[0] << " | Last occurrence: " << result[1] << endl;

    return 0;
}
  