#include <iostream>
#include <algorithm>
#include <map>
#include <unordered_map>
#include <queue>
#include <set>
#include <vector>
#include <fstream>
#include <sstream>
#include <string.h>
#include <memory>
#include <limits>
#include <list>
#include <regex>
#include <functional>
#include <math.h>
#include <unordered_set>
#define RED 0
#define BLACK 1
using namespace std;

struct TreeNode
{
    int val;
    bool color;
    struct TreeNode *left;
    struct TreeNode *right;
    struct TreeNode *parent;
    TreeNode(int val) : val(val), color(RED), left(nullptr), right(nullptr), parent(nullptr) {}
};

class RBT
{
    TreeNode* _root;
public:
    RBT();
    ~RBT();
    void left_rotate(TreeNode* n);
    void right_rotate(TreeNode* n);
    void insert_node(TreeNode*);
    void insert_fixup(TreeNode*);
    void preOrder(TreeNode*) const;
    void inOrder(TreeNode*) const;
    TreeNode* getRoot()const { return _root; }
};

RBT::RBT()
:_root(nullptr)
{
    cout << "RBT()" << endl;
}

RBT::~RBT()
{
    cout << "~RBT()" << endl;
}

void RBT::preOrder(TreeNode* root) const
{
    if (root)
    {
        cout << root->val << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

void RBT::inOrder(TreeNode* root) const
{
    if (root)
    {
        inOrder(root->left);
        cout << root->val << " ";
        inOrder(root->right);
    }
}


void RBT::left_rotate(TreeNode* x)
{
    TreeNode* y = x->right;
    x->right = y->left;
    if (y->left)
        y->left->parent = x;

    y->parent = x->parent;
    if (_root == x)
        _root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;

    y->left = x;
    x->parent = y;
}


void RBT::right_rotate(TreeNode* x)
{
    TreeNode* y = x->left;
    x->left = y->right;
    if (y->right)
        y->right->parent = x;
    
    y->parent = x->parent;
    if (_root == x)
        _root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else 
        x->parent->right = y;
    
    y->right = x;
    x->parent = y;
}

void RBT::insert_node(TreeNode* node)
{
    TreeNode* curr = _root, *pre = nullptr;
    while (curr)
    {
        pre = curr;
        if (node->val < curr->val)
            curr = curr->left;
        else if (node->val > curr->val)
            curr = curr->right;
        else //已经有了该值，不需要再添加
            return;
    }
    node->parent = pre;
    if (!pre)
        _root = node;
    else{
        if (node->val < pre->val)
            pre->left = node;
        else
            pre->right = node;
    }
    insert_fixup(node);
}


/*
通过红黑树的性质我们了解到该树对黑色节点的要求较为严格，所以我们在插入时选择插入红色的节点。
插入的几种情况：
1.插入节点后，父节点为空，说明此树原本一颗空树，只需要将节点颜色染黑即可；
2.插入节点后，父节点为黑色，并不会破坏红黑树的性质，则什么都不用做；
3.插入节点后，父节点为红色，叔节点存在并且也为红色，那么将 祖父节点，父节点，叔节点颜色翻转，并检查祖父节点是否为根节点；
4.插入节点后，父节点为红色，叔节点不存在，该种情况较为复杂：
1）如果插入的节点是在父节点的左边，那么将祖父节点向右旋转，并将父节点与祖父节点的颜色翻转即可；
2）如果插入的节点是在父节点的右边，那么先向父节点左旋转，再执行1）操作即可。

在维基百科和许多博客中将第五种情况： 父节点为红色，叔节点为黑色的情况也进行了讨论，不过我个人认为没有必要，因为这种前提条件下的红黑树本身已经违反了它的性质（第五条）。

*/

void RBT::insert_fixup(TreeNode* node)
{
    TreeNode* parent, *gparent, *uncle;
    //只有情况3，情况4才需要我们进行调整
    while ((parent=node->parent)!=nullptr && parent->color==RED)
    {
        gparent = parent->parent;
        if (parent == gparent->left){//父节点在左边，左右对称，下面的另一种情况是相同的
            //情况3
            if ((uncle=gparent->right)!=nullptr && uncle->color==RED){
                parent->color = BLACK;
                uncle->color = BLACK;
                gparent->color = RED;
                node = gparent;
                continue; //将祖父当成新插入的节点进行调整
            }
            //情况4
            if (parent->right == node){//可能是情况4中的2）情况，那么先进行一次旋转
                left_rotate(parent);
                TreeNode* t = parent;
                parent = node;
                node = t;
            }
            //情况4中1）情况
            parent->color = BLACK;
            gparent->color = RED;
            right_rotate(gparent);

        }else //对称操作
        {
            //情况3
            if ((uncle=gparent->left)!=nullptr && uncle->color==RED){
                parent->color = BLACK;
                uncle->color = BLACK;
                gparent->color = RED;
                node = gparent;
                continue; //将祖父当成新插入的节点进行调整
            }
            //情况4
            if (parent->left == node){//可能是情况4中的2）情况，那么先进行一次旋转
                right_rotate(parent);
                TreeNode* t = parent;
                parent = node;
                node = t;
            }
            //情况4中1）情况
            parent->color = BLACK;
            gparent->color = RED;
            left_rotate(gparent);
        }
    }
    _root->color = BLACK;
}



/*
删除节点：
1.删除节点有两个子节点，可以转化成删除单个子节点或者叶子节点的情况；
2.删除节点有单个子节点，那么待删除节点必为黑色，子节点必为红色，将子节点顶上去，再染成黑色即可；
3.删除叶子节点，该叶子节点为红色，那么直接删除即可；
4.删除叶子节点，该叶子节点为黑色，该叶子节点一定有兄弟节点，这种情况比较复杂，需要分情况讨论：
1）如果兄弟节点为红色，将叶子节点删除后，旋转后。
2）如果兄弟节点为黑色且为叶子节点，那么将叶子节点删除后，将父节点染黑，兄弟节点染红即可；
3）如果兄弟节点为黑色且不为叶子节点，那么兄弟节点的孩子一定为红色，又有以下两种情况：
一.兄弟节点左孩子为红色，经过如下变换：
二.兄弟节点右孩子为红色，经过如下变换：


*/




/*******************************测试函数********************************************/
void test1()
{
    RBT tree;
    tree.insert_node(new TreeNode(7));
    tree.insert_node(new TreeNode(2));
    tree.insert_node(new TreeNode(3));
    tree.insert_node(new TreeNode(1));
    tree.insert_node(new TreeNode(5));
    tree.insert_node(new TreeNode(6));
    tree.insert_node(new TreeNode(4));
    tree.insert_node(new TreeNode(8));
    cout << "前序遍历:" << endl;
    tree.preOrder(tree.getRoot());
    cout << "中序遍历:" << endl;
    tree.inOrder(tree.getRoot());
}

void test2()
{

}

int main()
{
    test1();
    return 0;
}
