//vector<int> arr = {5, 6, 7, 8, 9, 10, 1, 2, 3};

#include<iostream>
#include<vector>
using namespace std;

int binarySearch(vector<int> arr, int target, int size)
{
    int low = 0;
    int high = size-1;

    while(low <= high)
    {

        int mid = (low+high)/2;

        if(arr[mid] == target)
        {
            return mid;
        }
        // Check if left half is sorted
        else if(arr[low] <= arr[mid])
        {
            // Check if target is in the sorted left half
            if(target >= arr[low] && target < arr[mid])
                high = mid - 1;
            else
                low = mid + 1;
        }
        // Right half is sorted
        else
        {
            // Check if target is in the sorted right half
            if(target > arr[mid] && target <= arr[high])
                low = mid + 1;
            else
                high = mid - 1;
        }
    }
    return -1;
}

int main()
{
    vector<int> arr = {5, 6, 7, 8, 9, 10, 1, 2, 3};
    int key = 2;

    int index = binarySearch(arr, 2, arr.size());
    cout <<" The index value is : " << index <<endl;
    if(index>=0)
        cout <<" The key value found in :" << index <<endl;
    else
        cout <<" The key value not found in the array"<<endl;




    return 0;
}