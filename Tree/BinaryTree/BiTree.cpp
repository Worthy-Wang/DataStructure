#include <iostream>
#include <string>
#include <queue>
#include <stack>
#include <unistd.h>
using namespace std;

/*****************************二叉树**********************************/
/*****************************类申明**********************************/
struct Node
{
    char data;
    struct Node *left;
    struct Node *right;
    Node(int data = 0) : data(data), left(NULL), right(NULL) {}
};

class BiTree
{
    Node *_root;
    int _i;

public:
    BiTree(const char *s); //前序创建
    ~BiTree();             //后序销毁

    void preTraverse(Node *);               //前序遍历
    void inTraverse(Node *);                //中序遍历
    void postTraverse(Node *);              //后序遍历
    void levelTraverse(Node *);             //层序遍历
    void preTraverse_notRecursion(Node *);  //前序遍历非递归
    void inTraverse_notRecursion(Node *);   //中序遍历非递归
    void postTraverse_notRecursion(Node *); //后序遍历非递归

    Node *getRoot() const { return _root; }
    int getHeight(Node *root)
    {
        if (!root)
            return 0;
        else
            return std::max(getHeight(root->left), getHeight(root->right)) + 1;
    }

    int getBF(Node *root)
    {
        if (!root)
            return 0;
        return getHeight(root->left) - getHeight(root->right);
    }

private:
    Node *createBiTree(const char *s); //前序创建二叉树
    void destroyBiTree(Node *);        //后序销毁二叉树
};

/*****************************类成员定义**********************************/
BiTree::BiTree(const char *s)
{
    _i = 0;
    _root = createBiTree(s);
    cout << "BiTree()" << endl;
}

BiTree::~BiTree()
{
    if (_root)
    {
        destroyBiTree(_root);
        cout << "~BiTree()" << endl;
    }
}

void BiTree::destroyBiTree(Node *root)
{
    if (!root)
        return;
    else
    {
        destroyBiTree(root->left);
        destroyBiTree(root->right);
        delete root;
        root = NULL;
    }
}

Node *BiTree::createBiTree(const char *s)
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
        node->left = createBiTree(s);
        node->right = createBiTree(s);
        return node;
    }
}

void BiTree::preTraverse(Node *node)
{
    if (!node)
        cout << "#";
    else
    {
        cout << node->data;
        preTraverse(node->left);
        preTraverse(node->right);
    }
}

void BiTree::inTraverse(Node *node)
{
    if (!node)
        cout << "#";
    else
    {
        inTraverse(node->left);
        cout << node->data;
        inTraverse(node->right);
    }
}

void BiTree::postTraverse(Node *node)
{
    if (!node)
        cout << "#";
    else
    {
        postTraverse(node->left);
        postTraverse(node->right);
        cout << node->data;
    }
}

void BiTree::levelTraverse(Node *node)
{
    queue<Node *> que;
    que.push(node);
    while (!que.empty())
    {
        Node *temp = que.front();
        cout << temp->data;
        que.pop();
        if (temp->left)
            que.push(temp->left);
        if (temp->right)
            que.push(temp->right);
    }
}

//先不考虑打印，左右遍历完 , 先分别写出向左的，向右的
void BiTree::preTraverse_notRecursion(Node *root)
{
    stack<Node *> mystack;
    Node *p = root;
    while (p || !mystack.empty())
    {
        //向左
        while (p)
        {
            cout << p->data;
            mystack.push(p);
            p = p->left;
        }

        //向右
        if (!mystack.empty())
        {
            p = mystack.top();
            mystack.pop();
            p = p->right;
        }
    }
}

//先不考虑打印，左右遍历完 , 先分别写出向左的，向右的
void BiTree::inTraverse_notRecursion(Node *root)
{
    stack<Node *> mystack;
    Node *p = root;
    while (p || !mystack.empty())
    {
        //向左
        while (p)
        {
            mystack.push(p);
            p = p->left;
        }

        //向右
        if (!mystack.empty())
        {
            p = mystack.top();
            mystack.pop();
            cout << p->data;
            p = p->right;
        }
    }
}

//后序遍历非递归采用双栈法，依次弹栈
void BiTree::postTraverse_notRecursion(Node *root)
{
    stack<Node *> stack1;
    stack<Node *> stack2;
    stack1.push(root);
    while (!stack1.empty())
    {
        Node *temp = stack1.top();
        stack1.pop();
        if (temp->left)
            stack1.push(temp->left);
        if (temp->right)
            stack1.push(temp->right);

        stack2.push(temp);
    }

    //打印
    while (!stack2.empty())
    {
        Node *temp = stack2.top();
        stack2.pop();
        cout << temp->data;
    }
}

/*********************************测试程序******************************/

//四种遍历方法 前中后层
void test1()
{
    BiTree tree("ABD##E##C#G##");

    cout << "PreOrder:" << endl;
    tree.preTraverse(tree.getRoot());
    cout << endl;
    tree.preTraverse_notRecursion(tree.getRoot());
    cout << endl;

    cout << "InOrder:" << endl;
    tree.inTraverse(tree.getRoot());
    cout << endl;
    tree.inTraverse_notRecursion(tree.getRoot());
    cout << endl;

    cout << "PostOrder:" << endl;
    tree.postTraverse(tree.getRoot());
    cout << endl;
    tree.postTraverse_notRecursion(tree.getRoot());
    cout << endl;

    cout << "levelOrder:" << endl;
    tree.levelTraverse(tree.getRoot());
    cout << endl;
}

void test2()
{
    BiTree tree("1234####2#3#4##");
    cout << "PreOrder:" << endl;
    tree.preTraverse(tree.getRoot());
    cout << endl;
    tree.preTraverse_notRecursion(tree.getRoot());
    cout << endl;

    cout << "InOrder:" << endl;
    tree.inTraverse(tree.getRoot());
    cout << endl;
    tree.inTraverse_notRecursion(tree.getRoot());
    cout << endl;

    cout << "Height:" << tree.getHeight(tree.getRoot()) << endl;
    cout << "bf:" << tree.getBF(tree.getRoot()) << endl;
}

int main()
{
    test2();
    return 0;
}
