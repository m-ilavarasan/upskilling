#include<iostream>
#include<vector>

using namespace std;



int main()
{
    vector<int> arr = {1,2,3,4,3,5,3,6};
    int val = 3;
    /** /
    vector<int> result;
    for(int i = 0; i<arr.size(); i++)
    {
        if(arr[i] != val)
        {
            result.push_back(arr[i]);
        }
    }
    cout << result.size() << endl;
    /* */
    int i,j=0;
    for(i = 0; i< arr.size(); i++)
    {
        if(arr[i] != val)
        {
            arr[j++] = arr[i];
        }
    }
    cout << j << "&&" << arr.size() <<endl;
        return 0;
}
