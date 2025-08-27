/* https://leetcode.com/problems/next-permutation/ */\
#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

class Solution {
public:
    void nextPermutation(vector<int>& nums) {
        size_t n = nums.size();
        for (int i=1; i<n; i++) {
            if (nums[n-1-i] < nums[n-i]) {
                for (int k=1; k<=i; k++) {
                    // in place swapping of numbers
                    if (nums[n-k] > nums[n-1-i]) {
                        nums[n-1-i] = nums[n-1-i] ^ nums[n-k];
                        nums[n-k] = nums[n-1-i] ^ nums[n-k];
                        nums[n-1-i] = nums[n-1-i] ^ nums[n-k];
                        break;
                    }
                }
                // sort the remaining part
                sort(nums.begin()+n-i,nums.end());
                return;
            }
        }
        sort(nums.begin(),nums.end());
        return;
    }
};

int main() {
    //testing how sort works with offset :P
    vector<int> input = {4,5,2,3};
    sort(input.begin()+1,input.end());
    for (int val:input) cout << val << " ";
}