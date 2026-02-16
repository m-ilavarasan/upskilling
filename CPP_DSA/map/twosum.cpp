#include<iostream>
#include<unordered_map>
#include<vector>
using namespace std;

vector<int> twoSum(vector<int>& nums, int target)
{
    unordered_map<int, int> map1;
    
    for(int i = 0; i < nums.size(); i++)
    {
        int complement = target - nums[i];
        
        // Check if complement exists in map
        if(map1.find(complement) != map1.end())
        {
            return {map1[complement], i};
        }
        
        // Add current number and its index to map
        map1[nums[i]] = i;
    }
    
    return {}; // Return empty vector if no solution found
}

int main()
{
    vector<int> nums = {2, 7, 6, 3};
    int target = 9;
    
    vector<int> result = twoSum(nums, target);
    
    if(result.size() == 2)
    {
        cout << result[0] << " " << result[1] << endl;
    }
    else
    {
        cout << "No solution found" << endl;
    }
    
    return 0;
}
