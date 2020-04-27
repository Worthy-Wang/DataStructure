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
/*
            找到该节点，有四种情况：
            1.为叶子节点
            2.只有右子树
            3.只有左子树
            4.同时有左子树和右子树
*/

class Solution
{
public:
    TreeNode *deleteNode(TreeNode *root, int key)
    {
        //找到该节点,没找到返回NULL
        if (!root)
            return NULL;
        else if (key < root->val)
        {
            root->left = deleteNode(root->left, key);
            return root;
        }
        else if (key > root->val)
        {
            root->right = deleteNode(root->right, key);
            return root;
        }
        else
        {
            //叶子节点
            if (!root->left && !root->right)
            {
                delete root;
                return NULL;
            }
            //只有右子树
            if (!root->left && root->right)
            {
                TreeNode *temp = root->right;
                delete root;
                return temp;
            }
            //只有左子树
            else if (!root->right && root->left)
            {
                TreeNode *temp = root->left;
                delete root;
                return temp;
            }
            else
            {
                //左右子树均存在
                TreeNode *temp = root->right;
                while (temp->left)
                    temp = temp->left;
                root->val = temp->val;
                root->right = deleteNode(root->right, temp->val);
                return root;
            }
        }
    }
};

int main()
{

    return 0;
}