#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
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
    AVLTree();
    AVLTree(string s);
    ~AVLTree();

    void preTraverse(TreeNode *);              //前序遍历
    void inTraverse(TreeNode *);               //中序遍历AVL树，结果应由大到小
    TreeNode *search(TreeNode *, int val);     //查询操作，找到则返回节点，否则返回NULL
    TreeNode *getRoot() const { return _root; }
    TreeNode* insertNode(TreeNode *, int val);         //插入节点操作
    TreeNode *deleteNode(TreeNode *, int val); //删除操作
    void insertNode(int val);         //插入节点操作
    void deleteNode(int val); //删除操作

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

AVLTree::AVLTree()
:_root(nullptr)
{
    cout << "AVLTree()" << endl;
}

AVLTree::AVLTree(string s)
    : _root(NULL)
{
    cout << "AVLTree()" << endl;
    stringstream ss(s);
    int val;
    while (ss >> val)
        _root = insertNode(_root, val);
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
        cout << TreeNode->val << " ";
        preTraverse(TreeNode->left);
        preTraverse(TreeNode->right);
    }
}

void AVLTree::inTraverse(TreeNode *TreeNode)
{
    if (TreeNode)
    {
        inTraverse(TreeNode->left);
        cout << TreeNode->val << " ";
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


TreeNode* AVLTree::insertNode(TreeNode *root, int val)
{
    if (!root)
    {
        TreeNode* node = new TreeNode(val);
        return node;    
    }

    if (root->val == val)
    {
        return root; //AVL树中已经有该节点，不能够再插入
    }
    else if (val > root->val)
    {
        //向右插入
        root->right = insertNode(root->right, val);
        //插入新节点后如果失去平衡则调整
        if (getHeight(root->right) - getHeight(root->left) == 2)
        {
            TreeNode* t = root->right;
            if (getHeight(t->left) > getHeight(t->right))
                root = RL(root);
            else
                root = RR(root);
        }
    }else if (val < root->val)
    {
        //向左插入
        root->left = insertNode(root->left, val);
        //插入新节点后如果失去平衡则调整
        if (getHeight(root->left) - getHeight(root->right) == 2)
        {
            TreeNode* t = root->left;
            if (getHeight(t->left) > getHeight(t->right))
                root = LL(root);
            else
                root = LR(root);
        }
    }
    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    return root;
}

void AVLTree::insertNode(int val)
{
    _root  = insertNode(_root, val);
}

void AVLTree::deleteNode(int val)
{
    _root = deleteNode(_root, val);
}

TreeNode *AVLTree::deleteNode(TreeNode *root, int key)
{
    if (!root)//该节点并不存在
        return nullptr;

    if (key < root->val)
    {
        //key在root左侧
        root->left = deleteNode(root->left, key);
        if (getHeight(root->right) - getHeight(root->left) == 2)
        {
            TreeNode* t = root->right;
            if (getHeight(t->right) > getHeight(t->left))
                root = RR(root);
            else
                root = RL(root);
        }
    }else if (key > root->val)
    {
        //key在root右侧
        root->right = deleteNode(root->right, key);
        if (getHeight(root->left) - getHeight(root->right) == 2)
        {
            TreeNode* t = root->left;
            if (getHeight(t->left) > getHeight(t->right))
                root = LL(root);
            else
                root = LR(root);
        }
    }else
    {
        //找到要删除的节点key
        if (root->left && root->right)
        {
            if (getHeight(root->left) > getHeight(root->right))//从更高的子树开始删除，可以保证删除之后AVL仍然平衡
            {
                TreeNode* t = root->left;
                while(t->right)
                    t = t->right;
                root->val = t->val;
                root->left = deleteNode(root->left, t->val);
            }else
            {
                TreeNode* t = root->right;
                while(t->left)
                    t = t->left;
                root->val = t->val;
                root->right = deleteNode(root->right, t->val);
            }
        }else
        {
            TreeNode* t = root;
            root = root->left ? root->left : root->right;
            delete t;
            return root;
        }
    }

    root->height = std::max(getHeight(root->left), getHeight(root->right)) + 1;
    return root;
}




/**************************************测试函数****************************************/
void test1()
{
    AVLTree tree;
    tree.insertNode(3);
    // tree.deleteNode(3);
}

void test2()
{
    AVLTree tree("3 2 1 4 5 6 7 16 15 14 13 12 11 10 8 9");
    tree.preTraverse(tree.getRoot());
    cout << endl;
    tree.inTraverse(tree.getRoot());
    cout << endl;
    cout << "Height:" << tree.getHeight(tree.getRoot()) << endl;

}

void test3()
{
    AVLTree tree("3 2 1 4 5 6 7 16 15 14 13 12 11 10 8 9");
    tree.deleteNode(8);
    tree.preTraverse(tree.getRoot());
    cout << endl;
    tree.inTraverse(tree.getRoot());
    cout << endl;
    cout << "Height:" << tree.getHeight(tree.getRoot()) << endl;

}

int main()
{
    test3();
    return 0;
}
