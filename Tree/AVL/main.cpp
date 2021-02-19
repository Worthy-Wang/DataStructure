#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*********************************AVL(平衡二叉树)**************************************/
/*********************************类申明**************************************/
struct TreeNode
{
    int val;
    int height;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int val) : val(val), height(1), left(NULL), right(NULL) {}
};

class AVLTree
{
    TreeNode *_root;

public:
    AVLTree(const char *);
    ~AVLTree();

    void preTraverse(TreeNode *);              //前序遍历
    void inTraverse(TreeNode *);               //中序遍历AVL树，结果应由大到小
    TreeNode *search(TreeNode *, int val);     //查询操作，找到则返回节点，否则返回NULL
    TreeNode *getRoot() const { return _root; }
    TreeNode* insertNode(TreeNode *, int val);         //插入节点操作
    TreeNode *deleteNode(TreeNode *, int val); //删除操作

    //AVL树特有的成员变量
    TreeNode *RR(TreeNode *);    //左旋
    TreeNode *LL(TreeNode *);   //右旋
    TreeNode *LR(TreeNode *);   //先左旋再右旋
    TreeNode *RL(TreeNode *);   //先右旋再左旋
    int getHeight(TreeNode *);           //求树的高度
private:
    void destroyAVLTree(TreeNode *);
};

/*********************************类成员定义**************************************/

AVLTree::AVLTree(const char *str)
    : _root(NULL)
{
    cout << "AVLTree()" << endl;
    for (int i = 0; str[i] != '\0'; i++)
        _root = insertNode(_root, str[i] - 48);
}

AVLTree::~AVLTree()
{
    if (_root)
    {
        destroyAVLTree(_root);
        cout << "~AVLTree()" << endl;
    }
}

void AVLTree::destroyAVLTree(TreeNode *root)
{
    if (!root)
        return;
    else
    {
        destroyAVLTree(root->left);
        destroyAVLTree(root->right);
        delete root;
        root = NULL;
    }
}

void AVLTree::preTraverse(TreeNode *TreeNode)
{
    if (TreeNode)
    {
        cout << TreeNode->val;
        preTraverse(TreeNode->left);
        preTraverse(TreeNode->right);
    }
}

void AVLTree::inTraverse(TreeNode *TreeNode)
{
    if (TreeNode)
    {
        inTraverse(TreeNode->left);
        cout << TreeNode->val;
        inTraverse(TreeNode->right);
    }
}

TreeNode *AVLTree::search(TreeNode *p, int val)
{
    if (!p)
        return NULL;
    else if (val < p->val)
        return search(p->left, val);
    else if (val > p->val)
        return search(p->right, val);
    else
        return p;
}


int AVLTree::getHeight(TreeNode *root)
{
    if (!root)
        return 0;
    else
        return root->height;
}

TreeNode *AVLTree::RR(TreeNode *root)
{
    TreeNode* rchild = root->right;
    root->right = rchild->left;
    rchild->left = root;
    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    rchild->height = std::max(getHeight(rchild->left), getHeight(rchild->right)) + 1;
    return rchild;
}

TreeNode *AVLTree::LL(TreeNode *root)
{
    TreeNode* lchild = root->left;
    root->left = lchild->right;
    lchild->right = root;
    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    lchild->height = std::max(getHeight(lchild->left), getHeight(lchild->right)) + 1;
    return lchild;
}

TreeNode* AVLTree::LR(TreeNode* root)
{
    root->left = RR(root->left);
    return LL(root);
}

TreeNode* AVLTree::RL(TreeNode* root)
{
    root->right = LL(root->right);
    return RR(root);
}


TreeNode* AVLTree::insertNode(TreeNode *p, int val)
{
    if (!p)
    {
        TreeNode* node = new TreeNode(val);
        return node;    
    }
    if (p->val == val)
    {
        return p; //AVL树中已经有该节点，不能够再插入
    }
    else if (p->val < val)
    {
        //向右插入
        p->right = insertNode(p->right, val);
        //插入新节点后如果失去平衡则调整
        if ()
    }else if (p->val > val)
    {
        //向左插入
        p->left = insertNode(p->left, val);
        //插入新节点后如果失去平衡则调整
        
    }
    return p;
}


TreeNode *AVLTree::deleteNode(TreeNode *root, int key)
{

}




/**************************************测试函数****************************************/
void test1()
{
    AVLTree tree("3265879");
    tree.preTraverse(tree.getRoot());
    cout << endl;
    tree.inTraverse(tree.getRoot());
    cout << endl;
    cout << "Height:" << tree.getHeight(tree.getRoot()) << endl;
}

int main()
{
    test1();
    return 0;
}
