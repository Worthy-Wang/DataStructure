#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
/**
 * Definition for a binary tree node.
 */
struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};


/*
时间复杂度：O(n * logn) = O(nlogn)
空间复杂度：O(n)
*/

class Solution
{
public:
    int getHeight(TreeNode *root)
    {
        if (!root)
            return 0;
        else
            return std::max(getHeight(root->left), getHeight(root->right)) + 1;
    }

    int getBF(TreeNode *root)
    {
        if (!root)
            return 0;
        return getHeight(root->left) - getHeight(root->right);
    }

    bool isBalanced(TreeNode *root)
    {
        if (!root)
            return true;
        int factor = getBF(root);
        return (abs(factor)<=1) && isBalanced(root->left) && isBalanced(root->right);
    }
};

int main()
{

    return 0;
}