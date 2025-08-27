/* https://leetcode.com/problems/set-matrix-zeroes/description/ */

#include <vector>
#include <unordered_set>
using namespace std;


// intuitive O(m+n) space
class Solution {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = matrix[0].size();
        unordered_set<int> row_zeros;
        unordered_set<int> column_zeros;

        for (int i=0; i<m; i++) {
            for (int j=0; j<n; j++) {
                if (matrix[i][j] == 0) {
                    row_zeros.insert(i);
                    column_zeros.insert(j);
                }
            }
        }

        for (int val: row_zeros) {
            for (int k=0; k<n; k++) {
                matrix[val][k] = 0;
            }
        }
        for (int val: column_zeros) {
            for (int k=0; k<m; k++) {
                matrix[k][val] = 0;
            }
        }
    }
};


// O(1) space
class Solution2 {
public:
    void setZeroes(vector<vector<int>>& matrix) {
        int m = matrix.size();
        int n = matrix[0].size();
        bool row_flag = false;
        bool col_flag = false;

        for (int i=0; i<m; i++) {
            for (int j=0; j<n; j++) {
                if (matrix[i][j] == 0) {
                    matrix[0][j] = 0;
                    matrix[i][0] = 0;
                    if (i==0) row_flag = true;
                    if (j==0) col_flag = true;
                }
            }
        }

        for (int i=1; i<m; i++) {
            if (matrix[i][0] == 0)
                for (int k=1; k<n; k++) {
                    matrix[i][k] = 0;
                }
        }
        for (int j=1; j<n; j++) {
            if (matrix[0][j] == 0)
                for (int k=1; k<m; k++) {
                    matrix[k][j] = 0;
                }
        }

        if (col_flag) for (int i=0; i<m; i++) matrix[i][0] = 0;
        if (row_flag) for (int j=0; j<n; j++) matrix[0][j] = 0;
    }
};