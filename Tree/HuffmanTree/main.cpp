#include <iostream>
#include <queue>
#include <vector>
#include <functional>
using namespace std;

/*********************************HuffmanTree(霍夫曼树)****************************************/
/***************************************类申明****************************************/
struct TreeNode
{
    char val;   //节点中存放的值
    int weight; //权
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int weight) : weight(weight), left(NULL), right(NULL) {}
};

class HuffmanTree
{
    TreeNode *_root;

public:
    HuffmanTree(vector<int> &);
    void preTraverse(TreeNode *); //前序遍历
    void inTraverse(TreeNode *);  //中序遍历
    TreeNode *getRoot() const { return _root; }

private:
    TreeNode *createTree(vector<int> &);
};

/**************************************类定义****************************************/
HuffmanTree::HuffmanTree(vector<int> &vec)
{
    cout << "HuffmanTree()" << endl;
    _root = createTree(vec);
}

struct Mycompare
{
    bool operator()(const TreeNode *lhs, const TreeNode *rhs)
    {
        return lhs->weight > rhs->weight;
    }
};

TreeNode *HuffmanTree::createTree(vector<int> &vec)
{
    std::priority_queue<TreeNode *, vector<TreeNode *>, Mycompare> que;
    for (auto &e : vec)
    {
        TreeNode *node = new TreeNode(e);
        que.push(node);
    }
    while (que.size() > 1)
    {
        TreeNode *min1 = que.top();
        que.pop();
        TreeNode *min2 = que.top();
        que.pop();
        TreeNode *parent = new TreeNode(min1->weight + min2->weight);
        min1->weight < min2->weight ? (parent->left = min1, parent->right = min2) : (parent->right = min1, parent->left = min2);
        que.push(parent);
    }
    TreeNode *root = que.top();
    que.pop();
    return root;
}

void HuffmanTree::preTraverse(TreeNode *node)
{
    if (!node)
        cout << "# ";
    else
    {
        cout << node->weight << " ";
        preTraverse(node->left);
        preTraverse(node->right);
    }
}

void HuffmanTree::inTraverse(TreeNode *node)
{
    if (!node)
        cout << "# ";
    else
    {
        inTraverse(node->left);
        cout << node->weight << " ";
        inTraverse(node->right);
    }
}

/**********************************测试函数******************************************/
void test0()
{
    vector<int> weights{3, 2, 9, 7, 25, 18};
    HuffmanTree tree(weights);
    cout << "Pre:" << endl;
    tree.preTraverse(tree.getRoot());
    cout << endl;
    cout << "In:" << endl;
    tree.inTraverse(tree.getRoot());
    cout << endl;
}

int main()
{
    test0();
    return 0;
}
