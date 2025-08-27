/* https://leetcode.com/problems/maximum-subarray/description/ */

#include <vector>
using namespace std;

class Solution {
public:
    int maxSubArray(vector<int>& nums) 
    {
        int maxSums = nums[0];
        int currSums = nums[0];

        // generic standard Kadane's algorithm
        for (int i=1; i<nums.size(); ++i)
        {
            currSums = max(nums[i], currSums + nums[i]);
            maxSums = max(maxSums, currSums);
        }
        return maxSums;
    }

};