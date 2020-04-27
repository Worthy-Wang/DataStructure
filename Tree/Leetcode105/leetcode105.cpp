#include <iostream>
#include <vector>
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

class Solution
{
public:
    int findIndex(vector<int>& inorder, int val)
    {
        for (int i = 0; i < inorder.size(); i++)
            if (val == inorder[i])
                return i;
        return -1;
    }

    TreeNode *Build(vector<int> &preorder, vector<int> &inorder, int &i, int l, int r)
    {
        if (l <= r)
        {
            TreeNode *newNode = new TreeNode(preorder[i]);
            int mid = findIndex(inorder, preorder[i]);
            i++;
            newNode->left = Build(preorder, inorder, i, l, mid-1);
            newNode->right = Build(preorder, inorder, i, mid+1, r);
            return newNode;
        }
        else
        {
            return NULL;
        }
    }

    TreeNode *buildTree(vector<int> &preorder, vector<int> &inorder)
    {
        int l = 0, r = preorder.size() - 1;
        int i = 0;
        return Build(preorder, inorder, i, l, r);
    }
};


int main()
{
    vector<int> preorder{3,9,20,15,7};
    vector<int> inorder{9,3,15,20,7};
    Solution s;
    s.buildTree(preorder, inorder);
    return 0;
}