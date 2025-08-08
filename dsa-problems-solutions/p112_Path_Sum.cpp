// Problem 112 Path Sum
/*
Given the root of a binary tree and an integer targetSum, 
return true if the tree has a root-to-leaf path such that 
adding up all the values along the path equals targetSum.
A leaf is a node with no children.

input = [
    vector of values/null that sequentially give all the nodes of the tree from the left,
    for each depth.
], integer targetSum

*/

#include <iostream>
#include <vector>
#include <queue>
#include <optional>
using namespace std;

//definition
struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

//populate
TreeNode* buildTree(const vector<optional<int>>& vals) {
    if (vals.empty() || !vals[0].has_value()) 
        return nullptr;
    
    TreeNode* root = new TreeNode(*vals[0]);
    queue<TreeNode*> q;
    q.push(root);
    
    int i = 1, n = vals.size();
    while (!q.empty() && i < n) {
        TreeNode* node = q.front(); 
        q.pop();
        
        // left child
        if (i < n && vals[i].has_value()) {
            node->left = new TreeNode(*vals[i]);
            q.push(node->left);
        }
        i++;
        
        // right child
        if (i < n && vals[i].has_value()) {
            node->right = new TreeNode(*vals[i]);
            q.push(node->right);
        }
        i++;
    }
    
    return root;
}

//solve
bool hasPathSum(TreeNode* root, int targetSum) 
{
    if (!root) 
        return false;
    if (!root->left && !root->right) 
        return targetSum == root->val;
    return hasPathSum(root->left, targetSum-root->val)
        || hasPathSum(root->right, targetSum-root->val);
}

//exec
int main()
{
    //example 1
    vector<optional<int>> input = {5,4,8,11,nullopt,13,4,7,2,nullopt,nullopt,nullopt,1};
    int targetSum = 22;
    TreeNode* root = buildTree(input);

    cout << hasPathSum(root, targetSum);

    return 0;
}
