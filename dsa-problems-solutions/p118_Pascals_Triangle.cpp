/* https://leetcode.com/problems/pascals-triangle/description/ */

#include <vector>
using namespace std;

class Solution {
public:
    vector<vector<int>> generate(int numRows) {
        vector<vector<int>> pascal_triangle;
        for (int i=0; i<numRows; i++) {
            vector<int> vec(i+1,0);
            vec[0] = 1;
            vec[i] = 1;
            for (int j=1; j<i; j++) {
                vec[j] = pascal_triangle[i-1][j-1] + pascal_triangle[i-1][j];
            }
            pascal_triangle.push_back(vec);
        }
        return pascal_triangle;
    }
};