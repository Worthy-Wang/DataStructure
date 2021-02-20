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

struct Node{
    int val;
    bool color;
    struct Node* left;
    struct Node* right;
    struct Node* parent;
    Node():val(0), color(RED), left(nullptr), right(nullptr), parent(nullptr){}
};

//调用该函数，一定能够返回祖父节点
Node* grandparent(Node* n)
{
    return n->parent->parent;
}

//调用该函数，一定能返回叔节点
Node* uncle(Node* n)
{
    if (n->parent == grandparent(n)->left)
        return grandparent(n)->right;
    else
        return grandparent(n)->left;
}

Node* left_rotate(Node* n)
{
    Node* rchild = n->right;
    n->right = rchild->left;
    rchild->left = n;    
    return rchild;
}

Node* right_rotate(Node* n)
{
    Node* lchild = n->left;
    n->left = lchild->right;
    lchild->right = n;
    return lchild;   
}

/*
插入的情况
整个插入的过程进行情况的分析，情况由简单到复杂。
*/

//case1:插入空树 将该节点的颜色改成黑色即可
void insert_case1(Node* n)
{
    if (n->parent == nullptr)
        n->color = BLACK;
    else
        insert_case2(n);
}

//case2:插入的是黑色节点下方
void insert_case2(Node* n)
{
    if (n->parent->color == BLACK)
        return;
    else
        insert_case3(n);
}

//case3:插入红色节点下方 若该节点的父节点与叔节点都是红色，也就是 黑红红，那么改成红黑黑，重新进行case1判断
void insert_case3(Node* n)
{
    if (uncle(n) && uncle(n)->color==RED)
    {
        grandparent(n)->color = RED;
        n->color = BLACK;
        uncle(n)->color = BLACK;
        insert_case1(n);
    }else
        insert_case4(n);
}


//剩下的情况：插入红色节点下方，但是该节点的叔节点不存在或者为黑色的情况。余下的case4 和 case5 是需要一起使用的，我们将其看做一个整体：
//case4:判断位置进行左旋或者右旋
void insert_case4(Node* n)
{
    if (n==n->parent->right && n->parent==grandparent(n)->left)
    {
        left_rotate(n->parent);
        n = n->left;
    }
    else if (n==n->parent->left && n->parent==grandparent(n)->right)
    {
        right_rotate(n->parent);
        n = n->right;
    }
    insert_case5(n);
}

//case5:先染色，再判断位置进行左旋或者右旋
void insert_case5(Node* n)
{
    n->parent->color = BLACK;
    grandparent(n)->color = RED;
    if (n==n->parent->left && n->parent==grandparent(n)->left)
        right_rotate(grandparent(n));
    else if (n==n->parent->right && n->parent==grandparent(n)->right)
        left_rotate(grandparent(n));
}


/*
删除操作：
*/



int main()
{
   
   return 0;
}
