#include <iostream>
#include <vector>
using namespace std;

/*********************************BST(二叉排序树)**************************************/
/*********************************类申明**************************************/
struct TreeNode
{
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int val) : val(val), left(NULL), right(NULL) {}
};

class BSTree
{
    TreeNode *_root;

public:
    BSTree(const char *);
    ~BSTree();

    void inTraverse(TreeNode *);           //中序遍历二叉排序树，结果应由大到小
    void insert(TreeNode *&, int val);     //插入节点操作
    TreeNode *search(TreeNode *, int val); //查询操作，找到则返回节点，否则返回NULL
    TreeNode *deleteNode(TreeNode *, int val);

    TreeNode *getRoot() const { return _root; }

private:
    void destroyBSTree(TreeNode *);
};

/*********************************类成员定义**************************************/
BSTree::BSTree(const char *str)
    : _root(NULL)
{
    cout << "BSTree()" << endl;
    for (int i = 0; str[i] != '\0'; i++)
        insert(_root, str[i] - 48);
}

BSTree::~BSTree()
{
    if (_root)
    {
        destroyBSTree(_root);
        cout << "~BSTree()" << endl;
    }
}

void BSTree::destroyBSTree(TreeNode *root)
{
    if (!root)
        return;
    else
    {
        destroyBSTree(root->left);
        destroyBSTree(root->right);
        delete root;
        root = NULL;
    }
}

void BSTree::inTraverse(TreeNode *TreeNode)
{
    if (TreeNode)
    {
        inTraverse(TreeNode->left);
        cout << TreeNode->val;
        inTraverse(TreeNode->right);
    }
}

void BSTree::insert(TreeNode *&p, int val)
{
    if (!p)
        p = new TreeNode(val);
    else
    {
        if (val < p->val)
            insert(p->left, val);
        else if (val > p->val)
            insert(p->right, val);
        else
            return;
    }
}

TreeNode *BSTree::search(TreeNode *p, int val)
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

TreeNode *BSTree::deleteNode(TreeNode *root, int key)
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

/**************************************测试函数****************************************/
void test1()
{
    BSTree tree("628143");
    tree.inTraverse(tree.getRoot());
    cout << endl;
}

void test2()
{
    BSTree tree("536247");
    tree.deleteNode(tree.getRoot(), 3);
    tree.inTraverse(tree.getRoot());
    cout << endl;
}

int main()
{
    test2();
    return 0;
}
