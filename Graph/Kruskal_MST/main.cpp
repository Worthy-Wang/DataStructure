#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <stack>
#include <queue>
#include <climits>
#include <iomanip>
#include <algorithm>
using namespace std;

/*********************************图*********************************************/
/*
实现Kruskal算法，采用邻接矩阵形式创建图(无向图,连通图, 加权图)：
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

class Graph
{
    vector<vector<int>> _matrix; //邻接矩阵
    vector<ElemType> _vals;      //存放顶点元素
    vector<EdgeNode *> _edges;   //存放边的信息
    int _vertexNum;              //顶点数量
    int _edgeNum;                //边的数量
    vector<int> visited;         //判断定点是否访问过

public:
    Graph();
    void Prim_MST();       //Prim算法，求解最小生成树
    void Kruskal_MST();    //Kruskal算法，求解最小生成树
    void printAdjMatrix(); //测试邻接矩阵是否正确组合

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
            EdgeNode *edge = new EdgeNode(i, j, weight);
            _edges.push_back(std::move(edge));
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

int findRoot(vector<int> &union_find, int i)
{
    if (union_find[i] != -1)
        return findRoot(union_find, union_find[i]);
    return i;
}

//Kruskal求最小生成树算法 利用并查集依次合并
void Graph::Kruskal_MST()
{
    vector<int> union_find(_vertexNum, -1); //并查集

    std::sort(_edges.begin(), _edges.end(), [&](EdgeNode *lhs, EdgeNode *rhs) {
        return lhs->weight < rhs->weight;
    });

    for (auto &e : _edges)
    {
        int beg = e->beg;
        int end = e->end;

        int root1 = findRoot(union_find, beg);
        int root2 = findRoot(union_find, end);
        if (root1 != root2)
            union_find[end] = beg;
    }

    //  打印连接情况
    for (int i = 0; i < union_find.size(); i++)
        cout << _vals[union_find[i]] << "->" << _vals[i] << endl;
}


/*********************************测试函数*********************************************/

//Kruskal
void test1()
{
    Graph graph;
    graph.printAdjMatrix();
    graph.Kruskal_MST();
}

int main()
{
    test1();
    return 0;
}