#include <iostream>
#include <vector>

using namespace std;

int main()
{


    vector <int> nums = {0,0,1,1,1,2,2,3,3};

    int j = 0;
    int val = nums[0];
    nums[j] = nums[0];
    for(int i = 1; i < nums.size(); i++)
    {
        if(val != nums[i])
        {
            nums[j++] = nums[i];
            val = nums[i];
        }

    }
    for(int num:nums)
    {
        cout << num <<"\t";
    }
    cout <<" the array size should be after duplicate deletion is:" <<j <<endl;
    return 0;
}