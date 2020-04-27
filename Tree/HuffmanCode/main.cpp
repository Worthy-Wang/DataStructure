#include <iostream>
#include <queue>
#include <vector>
#include <functional>
#include <string>
#include <map>
using namespace std;

/*********************************HuffmanCode(霍夫曼编码)****************************************/
/***************************************类申明****************************************/
struct TreeNode
{
    char val;    //节点中存放的值
    int weight;  //权
    string code; //哈夫曼编码
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(char val, int weight) : val(val), weight(weight), left(NULL), right(NULL) {}
};

class HuffmanTree
{
    TreeNode *_root;
    map<string, char> _codemap;

public:
    HuffmanTree(vector<pair<char, int>> &);
    void preTraverse(TreeNode *); //前序遍历
    void inTraverse(TreeNode *);  //中序遍历
    TreeNode *getRoot() const { return _root; }

    void encode(TreeNode *root, string code);
    void printCodemap();

private:
    TreeNode *createTree(vector<pair<char, int>> &);
};

/**************************************类定义****************************************/
HuffmanTree::HuffmanTree(vector<pair<char, int>> &vec)
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

TreeNode *HuffmanTree::createTree(vector<pair<char, int>> &vec)
{
    std::priority_queue<TreeNode *, vector<TreeNode *>, Mycompare> que;
    for (auto &e : vec)
    {
        TreeNode *node = new TreeNode(e.first, e.second);
        que.push(node);
    }
    while (que.size() > 1)
    {
        TreeNode *min1 = que.top();
        que.pop();
        TreeNode *min2 = que.top();
        que.pop();
        TreeNode *parent = new TreeNode('\0', min1->weight + min2->weight);
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
        cout << node->val << " ";
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
        cout << node->val << " ";
        inTraverse(node->right);
    }
}

void HuffmanTree::encode(TreeNode *root, string code)
{
    if (root)
    {
        if (!root->left && !root->right)
        {
            root->code += code;
            _codemap[code] = root->val;
        }
        encode(root->left, code + "0");
        encode(root->right, code + "1");
    }
}

void HuffmanTree::printCodemap()
{
    for (auto &e : _codemap)
        cout << e.first << ":" << e.second << endl;
}

/**********************************测试函数******************************************/
//HuffmanTree
void test0()
{
    vector<pair<char, int>> vec{{'A', 2}, {'B', 3}, {'C', 7}, {'D', 9}, {'E', 18}, {'F', 25}};
    HuffmanTree tree(vec);
    cout << "Pre:" << endl;
    tree.preTraverse(tree.getRoot());
    cout << endl;
    cout << "In:" << endl;
    tree.inTraverse(tree.getRoot());
    cout << endl;
}

//HuffmanCode
void test1()
{
    //假设已经统计出了权重，创建最优二叉树
    vector<pair<char, int>> vec{{'A', 2}, {'B', 3}, {'C', 7}, {'D', 9}, {'E', 18}, {'F', 25}};
    HuffmanTree tree(vec);
    //编码
    string temp;
    tree.encode(tree.getRoot(), temp);
    tree.printCodemap();
    //解码
}

int main()
{
    test1();
    return 0;
}
