/* https://leetcode.com/problems/pascals-triangle-ii/ */

#include <vector>
using namespace std;

class Solution {
public:
    vector<int> getRow(int rowIndex) {
        vector<int> pascal_row(rowIndex+1,0);
        pascal_row[0] = 1;
        pascal_row[rowIndex] = 1;
        for (int i=1; i<=(rowIndex+1)/2; i++) {
            long long val = 1;
            for (int k=1; k<=i; k++) {
                val = val * (rowIndex - k + 1)/k;
            }
            pascal_row[i] = val;
            pascal_row[rowIndex-i] = val;
        }

        return pascal_row;
    }
};