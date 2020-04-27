#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>
#include <climits>
#include <iomanip>
#include <algorithm>
#include <map>
#include <set>
using namespace std;

/*********************************图*********************************************/
/*
实现Prim算法，采用邻接矩阵形式创建图(无向图,连通图, 加权图)：
_matrix 矩阵
*/

using ElemType = char;

/*********************************类申明*********************************************/
struct EdgeNode
{
    int beg;
    int end;
    int weight;
    EdgeNode(int beg, int end, int weight) : beg(beg), end(end), weight(weight) {}
};

struct Comp
{
    bool operator()(EdgeNode *lhs, EdgeNode *rhs)
    {
        return lhs->weight < rhs->weight;
    }
};

class Graph
{
    vector<vector<int>> _matrix;                //邻接矩阵
    vector<ElemType> _vals;                     //存放顶点元素
    vector<EdgeNode *> _edges;                  //存放边的信息
    map<int, set<EdgeNode *, Comp>> _vertexMap; //存放相应顶点的边信息
    int _vertexNum;                             //顶点数量
    int _edgeNum;                               //边的数量
    vector<int> visited;                        //判断定点是否访问过

public:
    Graph();
    void Prim_MST();            //Prim算法，求解最小生成树
    void printAdjMatrix();      //测试邻接矩阵是否正确组合
    void printVertexAndEdges(); //测试顶点及其相邻边是否正确

    void resetVisited()
    {
        for (int i = 0; i < _vertexNum; i++)
            visited[i] = 0;
    }

private:
    void createGraph();
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
        _matrix.resize(_vertexNum);
        for (int i = 0; i < _vertexNum; i++)
            _matrix[i].resize(_vertexNum);
        for (int i = 0; i < _vertexNum; i++)
            for (int j = 0; j < _vertexNum; j++)
                if (i == j)
                    _matrix[i][j] = 0;
                else
                    _matrix[i][j] = INT_MAX;
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
            _vals.push_back(val);
        }
    }

    //接下来，进行边的连接
    {
        for (int k = 0; k < _edgeNum; k++)
        {
            getline(ifs, str);
            stringstream ss(str);
            int i, j, weight; //用边连接的两个顶点的编号
            ss >> i >> j >> weight;
            _matrix[i][j] = weight;
            _matrix[j][i] = weight;

            //无向图存储边的时候仍然是只存放一条边 遵循从小指向大的原则
            EdgeNode *edge = new EdgeNode(i, j, weight);
            _edges.push_back(std::move(edge));
            _vertexMap[i].insert(std::move(edge));
        }
    }

    ifs.close();
}

void Graph::printAdjMatrix()
{
    cout << "-----------------------------------" << endl;
    for (int i = 0; i < _vertexNum; i++)
    {
        for (int j = 0; j < _vertexNum; j++)
        {
            if (INT_MAX == _matrix[i][j])
                cout << "  ∞  ";
            else
                cout << "  " << _matrix[i][j] << "  ";
        }
        cout << endl;
    }
    cout << "-----------------------------------" << endl;
}

void Graph::printVertexAndEdges()
{
    for (int i = 0; i < _vertexNum; i++)
    {
        cout << _vals[i] << " : ";
        set<EdgeNode *, Comp> edges = _vertexMap[i];
        for (auto &e : edges)
        {
            cout << e->beg << "->" << e->end << " (" << e->weight << ")   ";
        }
        cout << endl;
    }
}


//Prim算法的核心：不断在已经加入的顶点中找到最小的边，借此找到下一个顶点
void Graph::Prim_MST()
{
    vector<int> union_find(_vertexNum, -1);             //并查集，用来记录已加入的节点
    set<EdgeNode *, Comp> merged_edges = _vertexMap[0]; //先加入下表为0的顶点

    for (int i = 0; i < _vertexNum - 1; i++)
    {
        EdgeNode *min_edge = *merged_edges.begin();
        merged_edges.erase(merged_edges.begin());
        int min_beg = min_edge->beg;
        int min_end = min_edge->end;
        int nextVertex = min_end;
        union_find[min_end] = min_beg;
        set<EdgeNode *, Comp> newEdges = _vertexMap[nextVertex];
        for (auto &e : newEdges)
            merged_edges.insert(e);
    }

    //print union_find
    for (int i = 0; i < _vertexNum; i++)
        cout << _vals[union_find[i]] << "->" << _vals[i] << endl;
}

/*********************************测试函数*********************************************/

//Prim
void test1()
{
    Graph graph;
    graph.printAdjMatrix();
    graph.printVertexAndEdges();
    graph.Prim_MST();
}

int main()
{
    test1();
    return 0;
}