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
    void delete_node(TreeNode*);
    void delete_fixup(TreeNode*, TreeNode*);
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
一.删除节点有两个子节点，可以转化成删除单个子节点或者叶子节点的情况；

二.删除节点有单个子节点，那么待删除节点必为黑色，子节点必为红色，将子节点顶上去，再染成黑色即可；

三.删除叶子节点，该叶子节点为红色，那么直接删除即可；

四.删除叶子节点，该叶子节点为黑色，该叶子节点一定有兄弟节点，这种情况比较复杂，需要分情况讨论：（此时只讨论叶子节点在左侧的情况，右侧属于对称情况）

1. 如果兄弟节点为红色，那么将兄弟节点染黑，兄弟节点的左儿子染红，再左转即可。

2. 如果兄弟节点为黑色且为叶子节点，那么将兄弟节点染红，由于此时黑色节点数目不均衡，所以需要再次对父节点进行调整；

3. 如果兄弟节点为黑色且不为叶子节点，那么兄弟节点的孩子一定为红色，又有以下三种情况：
1）兄弟节点有一个红色右孩子，经过如下变换：
将父节点的颜色赋值给兄弟节点，再将父节点，兄弟节点的右孩纸颜色染成黑色，最后左旋

2）兄弟节点有一个红色左孩子，经过如下变换：
将兄弟节点染红，兄弟节点左孩子染黑，最后对兄弟节点右转，也就回到了1）的情况

3）兄弟节点有两个红色孩子，经过如下变换：
该情况实质上也和情况1）是一样的，将父节点的颜色赋值给兄弟节点，再将父节点，兄弟节点的右孩纸颜色染成黑色，最后左旋

*/

void RBT::delete_node(TreeNode* node)
{
    //replace代表删除之后顶替上来的节点
    //parent为replace节点的父节点
    TreeNode* replace = nullptr, *parent = nullptr;
    //如果删除的节点有左右孩子，那么找到左子树最大的孩子或者右子树最小的孩子进行替换
    if (node->left && node->right){
        TreeNode* succ = nullptr;
        for (succ = node->left; succ->right; succ = succ->right);
        node->val = succ->val;
        delete_node(succ);
        return;
    }else{//删除节点为叶子节点 或者 只有一个孩子
        //如果删除根
        if (!node->parent){
            _root = (node->left ? node->left : node->right);
            replace = _root;
            if (_root)
                _root->parent = nullptr;
        }else{//删除的不是根
            TreeNode* child = (node->left ? node->left : node->right);
            if  (node->parent->left == node)
                node->parent->left = child;
            else
                node->parent->right = child;
            
            if (child)
                child->parent = node->parent;
            replace = child;
            parent = node->parent;
        }
    }

    //如果删除节点为红色，直接结束
    if (node->color == BLACK)
        delete_fixup(replace, parent);
}


void RBT::delete_fixup(TreeNode* replace,  TreeNode* parent)
{
    TreeNode* brother = nullptr;
    // 如果顶替结点是黑色结点，并且不是根结点。
    //由于经过了上面的deleteNode方法，这里面parent是一定不为null的
    while ((replace == nullptr || replace->color == BLACK) && replace != this->_root){
        //左孩子位置的所有情况，
        if (parent->left == replace) {
            brother = parent->right;
            // case1 红兄，brother涂黑，parent涂红，parent左旋，replace的兄弟改变了，变成了黑兄的情况
            if (brother->color == RED) {
                brother->color = BLACK;
                parent->color = RED;
                left_rotate(parent);
                brother = parent->right;
            }
            // 经过上面，不管进没进if，兄弟都成了黑色
            // case2 黑兄，且兄弟的两个孩子都为黑
            if ((brother->left == nullptr || brother->left->color == BLACK) && (brother->right == nullptr || brother->right->color == BLACK)) {
            // 如果parent此时为红，则把brother的黑色转移到parent上
                if (parent->color == RED) {
                parent->color = BLACK;
                brother->color = RED;
                break;
            } else {// 如果此时parent为黑，即此时全黑了，则把brother涂红，导致brother分支少一个黑，使整个分支都少了一个黑，需要对parent又进行一轮调整
                brother->color = RED;
                replace = parent;
                parent = replace->parent;
            }
        } else {
            // case3 黑兄，兄弟的左孩子为红色
            if (brother->left != nullptr && brother->left->color == RED) {
                brother->left->color = parent->color;
                parent->color = BLACK;
                right_rotate(brother);
                left_rotate(parent);
            // case4 黑兄，兄弟的右孩子为红色
            } else if (brother->right != nullptr && brother->right->color == RED) {
                brother->color = parent->color;
                parent->color = BLACK;
                brother->right->color = BLACK;
                left_rotate(parent);
            }
            break;
        }
    } else {//对称位置的情况，把旋转方向反回来
        brother = parent->left;
        // case1 红兄，brother涂黑，parent涂红，parent左旋，replace的兄弟改变了，变成了黑兄的情况
        if (brother->color == RED) {
            brother->color = BLACK;
            parent->color = RED;
            right_rotate(parent);
            brother = parent->left;
        }
        // 经过上面，不管进没进if，兄弟都成了黑色
        // case2 黑兄，且兄弟的两个孩子都为黑
        if ((brother->left == nullptr || brother->left->color == BLACK)
            && (brother->right == nullptr || brother->right->color == BLACK)) {
        // 如果parent此时为红，则把brother的黑色转移到parent上
            if (parent->color == RED) {
                parent->color = BLACK;
                brother->color = RED;
                break;
            } else {// 如果此时parent为黑，即此时全黑了，则把brother涂红，导致brother分支少一个黑，使整个分支都少了一个黑，需要对parent又进行一轮调整
                brother->color = RED;
                replace = parent;
                parent = replace->parent;
                }
            } else {
                // case3 黑兄，兄弟的左孩子为红色，右孩子随意
                if (brother->right != nullptr && brother->right->color == RED) {
                    brother->right->color = parent->color;
                    parent->color = BLACK;
                    left_rotate(brother);
                    right_rotate(parent);
                // case4 黑兄，兄弟的右孩子为红色，左孩子随意
                } else if (brother->left != nullptr && brother->left->color == RED) {
                    brother->color = parent->color;
                    parent->color = BLACK;
                    brother->left->color = BLACK;
                    right_rotate(parent);
                }
                break;
            }
        }
    }
    //这里可以处理到删除结点为只有一个孩子结点的情况，如果是根，也会将其涂黑。
    if (replace != nullptr)
        replace->color = BLACK;
}

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
    RBT tree;
    tree.insert_node(new TreeNode(7));
    tree.insert_node(new TreeNode(2));
    tree.insert_node(new TreeNode(3));
    tree.insert_node(new TreeNode(1));
    tree.insert_node(new TreeNode(5));
    tree.insert_node(new TreeNode(6));
    tree.insert_node(new TreeNode(4));
    tree.insert_node(new TreeNode(8));

    tree.delete_node(tree.getRoot());
    cout << "前序遍历:" << endl;
    tree.preOrder(tree.getRoot());
    cout << "中序遍历:" << endl;
    tree.inOrder(tree.getRoot());
}

int main()
{
    test2();
    return 0;
}
