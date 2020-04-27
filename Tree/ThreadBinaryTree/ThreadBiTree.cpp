#include <iostream>
using namespace std;

/*******************************线索二叉树***************************************/
/*
在线索二叉树中，没有空指针，创建线索二叉树的意义是更好的找到节点的前驱和后继。
整个线索二叉树都基于中序遍历。
*/

/**********************************类申明*****************************************/
struct Node
{
    char val;
    struct Node *left;
    struct Node *right;
    int ltag; //0表示正常左孩子，1表示前缀
    int rtag; //0表示正常右孩子，1表示后缀

    Node(char val) : val(val), left(NULL), right(NULL), ltag(0), rtag(0) {}
};

class ThreadBiTree
{
    Node *_root;
    int _i;
    Node *prev; //prev用来指向前一个节点，它最终会指向最后一个节点

public:
    ThreadBiTree(const char *s);    //前序创建, 线索二叉树由于没有空指针，所以无法进行后序销毁
    void inTraverse_setTag(Node *); //中序遍历设置tag
    void inTraverse_byTag(Node *);  //通过tag进行测试

    void inTraverse(Node *); //中序遍历测试程序，测试inTraverse_setTag有没有设置正确
    Node *getRoot() const { return _root; }

private:
    Node *createThreadBiTree(const char *s); //前序创建二叉树
};

/*******************************类成员定义***************************************/
ThreadBiTree::ThreadBiTree(const char *s)
{
    _i = 0;
    _root = createThreadBiTree(s);
    prev = new Node(-1); //头节点
    cout << "ThreadBiTree()" << endl;
}

Node *ThreadBiTree::createThreadBiTree(const char *s)
{
    if ('\0' == s[_i])
        return NULL;
    else if ('#' == s[_i])
    {
        _i++;
        return NULL;
    }
    else
    {
        Node *node = new Node(s[_i]);
        _i++;
        node->left = createThreadBiTree(s);
        node->right = createThreadBiTree(s);
        return node;
    }
}

void ThreadBiTree::inTraverse_setTag(Node *p)
{
    if (p)
    {

        inTraverse_setTag(p->left);

        if (!p->left)
        {
            p->ltag = 1;
            p->left = prev;
        }

        if (!prev->right)
        {
            prev->rtag = 1;
            prev->right = p;
        }
        prev = p;

        inTraverse_setTag(p->right);
    }
}

void ThreadBiTree::inTraverse(Node *node)
{
    if (node)
    {
        if (!node->ltag)
            inTraverse(node->left);
        cout << node->val;
        if (!node->rtag)
            inTraverse(node->right);
    }
}

void ThreadBiTree::inTraverse_byTag(Node *node)
{
    Node *p = node;

    while (p != prev)
    {
        //向左
        while (0 == p->ltag)
        {
            p = p->left;
        }
        cout << p->val;
        //向右
        while (1 == p->rtag)
        {
            p = p->right;
            cout << p->val;
        }
        p = p->right;
    }
    cout << prev->val;
}

/*****************************测试函数**********************************/
int main()
{
    ThreadBiTree tree("ABD##E##C#G##");
    tree.inTraverse_setTag(tree.getRoot());
    // tree.inTraverse(tree.getRoot());
    tree.inTraverse_byTag(tree.getRoot());
    return 0;
}