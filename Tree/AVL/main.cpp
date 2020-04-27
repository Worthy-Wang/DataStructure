#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

/*********************************AVL(平衡二叉树)**************************************/
/*********************************类申明**************************************/
struct TreeNode
{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int val) : val(val), left(NULL), right(NULL) {}
};

class AVLTree
{
    TreeNode *_root;

public:
    AVLTree(const char *);
    ~AVLTree();

    void preTraverse(TreeNode *);              //前序遍历
    void inTraverse(TreeNode *);               //中序遍历AVL树，结果应由大到小
    TreeNode* insert(TreeNode *, int val);         //插入节点操作
    TreeNode *search(TreeNode *, int val);     //查询操作，找到则返回节点，否则返回NULL
    TreeNode *deleteNode(TreeNode *, int val); //删除操作
    TreeNode *getRoot() const { return _root; }

    //AVL树特有的成员变量
    TreeNode *leftRotate(TreeNode *);    //左旋
    TreeNode *rightRotate(TreeNode *);   //右旋
    int getHeight(TreeNode *);           //求树的高度
    int getBF(TreeNode *);               //求该节点的平衡因子(balanced factor)
    TreeNode *balanceAdjust(TreeNode *); //AVL树平衡调节
private:
    void destroyAVLTree(TreeNode *);
};

/*********************************类成员定义**************************************/

AVLTree::AVLTree(const char *str)
    : _root(NULL)
{
    cout << "AVLTree()" << endl;
    for (int i = 0; str[i] != '\0'; i++)
        _root = insert(_root, str[i] - 48);
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

TreeNode* AVLTree::insert(TreeNode *p, int val)
{
    if (!p)
    {
        TreeNode* node = new TreeNode(val);
        return node;
    }
    else
    {
        if (val < p->val)
        {
            p->left = insert(p->left, val);
            return balanceAdjust(p);
        }   
        else if (val > p->val)
        {
            p->right = insert(p->right, val);
            return balanceAdjust(p);
        }
        else //该节点已经存在
            return p;
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

TreeNode *AVLTree::deleteNode(TreeNode *root, int key)
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

int AVLTree::getBF(TreeNode *root)
{
    if (!root)
        return 0;
    return getHeight(root->left) - getHeight(root->right);
}

int AVLTree::getHeight(TreeNode *root)
{
    if (!root)
        return 0;
    else
        return std::max(getHeight(root->left), getHeight(root->right)) + 1;
}

TreeNode *AVLTree::leftRotate(TreeNode *root)
{
    if (!root)
        return NULL;
    TreeNode *rchild = root->right;
    root->right = rchild->left;
    rchild->left = root;
    return rchild;
}

TreeNode *AVLTree::rightRotate(TreeNode *root)
{
    if (!root)
        return NULL;
    TreeNode *lchild = root->left;
    root->left = lchild->right;
    lchild->right = root;
    return lchild;
}

TreeNode *AVLTree::balanceAdjust(TreeNode *root)
{
    if (!root)
        return NULL;

    if (getBF(root) > 1 && getBF(root->left) > 0) //LL旋转
        return rightRotate(root);
    else if (getBF(root) > 1 && getBF(root->left) < 0) //LR旋转
    {
        TreeNode *lchild = root->left;
        root->left = leftRotate(lchild);
        return rightRotate(root);
    }
    else if (getBF(root) < -1 && getBF(root->right) < 0) //RR旋转
        return leftRotate(root);
    else if (getBF(root) < -1 && getBF(root->right) > 0) // RL旋转
    {
        TreeNode *rchild = root->right;
        root->right = rightRotate(rchild);
        return leftRotate(root);
    }
    else
    { //不需要进行调整
        return root;
    }
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
    cout << "BF:" << tree.getBF(tree.getRoot()) << endl;
}

int main()
{
    test1();
    return 0;
}
