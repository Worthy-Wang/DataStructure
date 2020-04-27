#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>
using namespace std;

/*********************************图*********************************************/
/*
采用邻接表形式创建图(有向图,连通图,加权图)：
vertex : 顶点
EdgeNode: 边节点

*/
using ElemType = char;
struct EdgeNode
{
    int index; // 用于寻找顶点位置的索引
    int weight;
    struct EdgeNode *next;
    EdgeNode(int index, int weight) : index(index), weight(weight), next(NULL) {}
};

struct Vertex
{
    int index;
    ElemType val;
    struct EdgeNode *adjNode;
    Vertex(int index, ElemType val) : index(index), val(val), adjNode(NULL) {}
};

/*********************************类申明*********************************************/
class Graph
{
    vector<Vertex *> _vertexs; //顶点
    int _vertexNum;            //顶点数量
    int _edgeNum;              //边的数量，注意是无向图
    vector<int> visited;       //判断定点是否访问过
public:
    Graph();
    void DFS(int);              //DFS 深度优先遍历
    void BFS(int);              //BFS 广度优先遍历
    void DFS_notRecursion(int); //DFS 深度优先遍历非递归
    int BFS_minPath(int, int);  //BFS 广度优先遍历 无权图求解两顶点之间的最短路径

    void printAdjList(); //测试邻接表是否正确组合
    Vertex *getRoot() const { return _vertexs[0]; }
    EdgeNode *firstNeighbor(int i)
    {
        if (i >= 0 && i <= _vertexNum - 1)
            return _vertexs[i]->adjNode;
        return NULL;
    }
    EdgeNode *nextNeighbor(EdgeNode *p)
    {
        if (!p)
            return NULL;
        return p->next;
    }

    void resetVisited()
    {
        for (int i = 0; i < _vertexNum; i++)
            visited[i] = 0;
    }

private:
    void createGraph();
    void insertEdgeNode(Vertex *v, EdgeNode *node); //插入边节点，用头插法
};

/*********************************类定义*********************************************/
Graph::Graph()
{
    createGraph();
    visited.resize(_vertexNum);
}

void Graph::createGraph()
{
    std::ifstream ifs("graph.conf", ios::in);
    string str;
    //第一行，读取图顶点的个数
    {
        getline(ifs, str);
        stringstream ss(str);
        ss >> _vertexNum;
    }

    //第二行，读取边的数量
    {
        getline(ifs, str);
        stringstream ss(str);
        ss >> _edgeNum;
    }

    //第三行, 读取顶点元素
    {
        ElemType val;
        getline(ifs, str);
        stringstream ss(str);
        for (int i = 0; i < _vertexNum; i++)
        {
            ss >> val;
            Vertex *v = new Vertex(i, val);
            _vertexs.push_back(v);
        }
    }

    //接下来，进行边的连接
    {
        for (int i = 0; i < _edgeNum; i++)
        {
            getline(ifs, str);
            stringstream ss(str);
            int index1, index2, weight;       //用边连接的两个顶点的编号
            ss >> index1 >> index2 >> weight; //有向加权图
            EdgeNode *newNode2 = new EdgeNode(index2, weight);
            insertEdgeNode(_vertexs[index1], newNode2);
        }
    }

    ifs.close();
}

void Graph::insertEdgeNode(Vertex *v, EdgeNode *node)
{
    node->next = v->adjNode;
    v->adjNode = node;
}

void Graph::printAdjList()
{
    for (int i = 0; i < _vertexNum; i++)
    {
        cout << "Vertex " << _vertexs[i]->val << " -> ";
        EdgeNode *n = firstNeighbor(i);
        while (n)
        {
            cout << _vertexs[n->index]->val << "(weight " << n->weight << ")"
                 << " -> ";
            n = nextNeighbor(n);
        }
        cout << "NULL" << endl;
    }
}

//使用邻接表的思想
void Graph::DFS(int index)
{
    cout << _vertexs[index]->val << " -> ";
    visited[index] = 1;
    EdgeNode *node = firstNeighbor(index); // 得到firstChild
    while (node)
    {
        if (!visited[node->index])
            DFS(node->index);
        else
            node = nextNeighbor(node);
    }
}

//使用树BFS的思想
void Graph::BFS(int index)
{
    std::queue<int> que;
    que.push(index);
    visited[index] = 1;
    cout << _vertexs[index]->val << " -> ";

    while (!que.empty())
    {
        int root = que.front();
        que.pop();
        EdgeNode *node = firstNeighbor(root);
        while (node)
        {
            if (!visited[node->index])
            {
                que.push(node->index);
                visited[node->index] = 1;
                cout << _vertexs[node->index]->val << " -> ";
            }
            node = nextNeighbor(node);
        }
    }
}

void Graph::DFS_notRecursion(int index)
{
    std::stack<int> mystack; //只存放顶点

    //根节点
    cout << _vertexs[index]->val << " -> ";
    mystack.push(index);
    visited[index] = 1;
    EdgeNode *p = firstNeighbor(index);

    while (1)
    {
        //向左(深度)
        while (p)
        {
            if (!visited[p->index])
            {
                cout << _vertexs[p->index]->val << " -> ";
                mystack.push(p->index);
                visited[p->index] = 1;
            }
            p = firstNeighbor(p->index);
        }

        //向右
        if (!mystack.empty())
        {
            int root = mystack.top();
            p = firstNeighbor(root);
            while (p != NULL)
            {
                if (!visited[p->index])
                    break;
                p = nextNeighbor(p);
            }
            if (p == NULL)
                mystack.pop();
        }
        else
        {
            break;
        }
    }
}

//BFS算法求解 两顶点之间的最短路径(仅限不带权的图)
//非权重图一般不会要求打印path，可以直接通过可视化图看出来
int Graph::BFS_minPath(int beg, int end)
{
    //辅助变量 distance 记录从beg顶点到每一个顶点的最短路径
    vector<int> distance;
    for (int i = 0; i < _vertexNum; i++)
        distance.push_back(65536);
    distance[beg] = 0;
    //辅助变量 队列
    std::queue<int> que;
    que.push(beg);
    visited[beg] = 1;

    while (!que.empty())
    {
        Vertex *v = _vertexs[que.front()];
        que.pop();
        EdgeNode *node = v->adjNode;
        while (node)
        {
            if (!visited[node->index])
            {
                que.push(node->index);
                visited[node->index] = 1;
                distance[node->index] = distance[v->index] + 1;
            }
            node = node->next;
        }
    }

    return distance[end];
}

/*********************************测试函数*********************************************/
//打印
void test0()
{
    Graph graph;
    graph.printAdjList();
    cout << "DFS:" << endl;
    graph.DFS(0);
    cout << endl;

    graph.resetVisited();
    cout << "DFS_notRecursion:" << endl;
    graph.DFS_notRecursion(0);
    cout << endl;

    graph.resetVisited();
    cout << "BFS:" << endl;
    graph.BFS(0);
    cout << endl;
}

//BFS最短路径
void test1()
{
    Graph graph;
    int beg = 0;
    int end = 10;
    cout << "Vertex " << beg << " to Vertex " << end << " mininum path is " << graph.BFS_minPath(beg, end) << endl;
}

int main()
{
    test0();
    return 0;
}