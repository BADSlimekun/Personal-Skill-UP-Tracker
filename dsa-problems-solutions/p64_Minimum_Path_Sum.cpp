#include <iostream>
#include <vector>

using namespace std;

int minPathSum(vector<vector<int>>& g) {
    if (g.empty() || g[0].empty()) return 0; // checks for empty cases or edge cases
    int n = g.size(), m = g[0].size(); 
    vector<int> dp(m, INT_MAX); // creating dp vector initialized with INT_MAX
    dp[0] = 0;
    for (int i = 0; i < n; ++i) { // iterate on all the rows
        dp[0] += g[i][0]; // adding the first element of that row in the 0th index for each n.
        for (int j = 1; j < m; ++j)
            dp[j] = min(dp[j], dp[j-1]) + g[i][j]; // for each row, looping on the rest of columns
            // initially it will conveniently store the first row of the grid, sequentially added up as went along the row
            // clearly the dp[j] stores now the shortest path to reach the i+1th row's j+1th column. So at the end, 
            // j = m-1, implies to reach the corner of the grid, it will take that much minimum cost.
    }
    return dp[m-1];
}

