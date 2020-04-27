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
    int findIndex(vector<int> &inorder, int val)
    {
        for (int i = 0; i < inorder.size(); i++)
            if (val == inorder[i])
                return i;
        return -1;
    }

    TreeNode *Build(vector<int> &preorder, vector<int> &inorder, int &i, int l, int r)
    {
        if (l <= r && i>=0)
        {
            TreeNode *newNode = new TreeNode(preorder[i]);
            int mid = findIndex(inorder, preorder[i]);
            i--;
            newNode->right = Build(preorder, inorder, i, mid + 1, r);
            newNode->left = Build(preorder, inorder, i, l, mid - 1);
            return newNode;
        }
        else
        {
            return NULL;
        }
    }

    TreeNode *buildTree(vector<int> &inorder, vector<int> &postorder)
    {
        int l = 0, r = postorder.size() - 1;
        int i = r;
        return Build(postorder, inorder, i, l, r);
    }
};

int main()
{
    vector<int> preorder{3, 9, 20, 15, 7};
    vector<int> inorder{9, 3, 15, 20, 7};
    Solution s;
    s.buildTree(preorder, inorder);
    return 0;
}