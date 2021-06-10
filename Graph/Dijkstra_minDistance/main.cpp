#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>
using namespace std;

/*********************************图*********************************************/
/*
因为使用Dijkstra算法中只可能是单节点向前推荐，所以采用有向图的方式（实际上无向图也可以，运算时看作有向图）
采用邻接表形式创建图(有向图,连通图, 加权图)：
vertex : 顶点
EdgeNode: 边节点

*/

using ElemType = char;
struct EdgeNode
{
    int index;  // 用于寻找顶点位置的索引
    int weight; //权
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
    int dijkstra_minDistance(int beg, int end); //Dijkstra算法 求解加权图的最短距离

    void printAdjList(); //测试邻接表是否正确组合
    Vertex *getRoot() const { return _vertexs[0]; }

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

    //第三行，读取顶点元素
    {
        getline(ifs, str);
        stringstream ss(str);
        ElemType val;
        for (int i = 0; i < _vertexNum; i++)
        {
            ss >> val;
            Vertex *newNode = new Vertex(i, val);
            _vertexs.push_back(newNode);
        }
    }

    //接下来，进行边的连接
    {
        for (int i = 0; i < _edgeNum; i++)
        {
            getline(ifs, str);
            stringstream ss(str);
            int index1, index2, weight; //用边连接的两个顶点的编号
            ss >> index1 >> index2 >> weight;
            EdgeNode *newNode = new EdgeNode(index2, weight);
            insertEdgeNode(_vertexs[index1], newNode);
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
        Vertex *v = _vertexs[i];
        EdgeNode *n = v->adjNode;
        while (n)
        {
            cout << _vertexs[n->index]->val << "(weight:" << n->weight << ")"
                 << " -> ";
            n = n->next;
        }
        cout << "NULL" << endl;
    }
}

//Dijkstra算法，本质上还是使用BFS遍历,只是在每一层判断路径和
int Graph:: dijkstra_minDistance(int beg, int end)
{
    vector<int> path;
    path.resize(_vertexNum);
    vector<int> distance;
    for (int i = 0; i < _vertexNum; i++)
        distance.push_back(65536);
    distance[beg] = 0;

    std::queue<int> que;
    que.push(beg);

    while (!que.empty())
    {
        Vertex *v = _vertexs[que.front()];
        que.pop();
        EdgeNode *node = v->adjNode;
        while (node)
        {
            que.push(node->index);
            if (distance[node->index] > distance[v->index] + node->weight)
            {
                distance[node->index] = distance[v->index] + node->weight;
                path[node->index] = v->index;
            }
            node = node->next;
        }
    }
    
    //打印path操作
    vector<ElemType> printPath;
    int i = end;
    while (i != beg)
    {
        printPath.push_back(_vertexs[i]->val);
        i = path[i];
    }
    printPath.push_back(_vertexs[beg]->val);
    for (auto it = printPath.rbegin(); it != printPath.rend(); it++)
        cout << *it << "->";
    cout << endl;
    
    return distance[end];
}

/*********************************测试函数*********************************************/
//打印
void test0()
{
    Graph graph;
    graph.printAdjList();
    int beg = 0;
    int end = 6;
    cout << graph.dijkstra_minDistance(beg, end) << endl;
}

int main()
{
    test0();
    return 0;
}