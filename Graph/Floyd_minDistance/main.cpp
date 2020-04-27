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
实现Floyd算法必须使用邻接矩阵的存储方式，所以采用邻接矩阵形式创建图(无向图,连通图, 加权图)：
_matrix 矩阵
*/

using ElemType = char;

/*********************************类申明*********************************************/
class Graph
{
    vector<vector<int>> _matrix; //邻接矩阵
    vector<ElemType> _vals;      //存放顶点元素
    int _vertexNum;              //顶点数量
    int _edgeNum;                //边的数量，注意是无向图
    vector<int> visited;         //判断定点是否访问过
public:
    Graph();
    void Floyd_minDistance(); //弗洛伊德算法 求解多源最短路径

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

void Graph::Floyd_minDistance()
{
    for (int k = 0; k < _vertexNum; k++)
        for (int i = 0; i < _vertexNum; i++)
            for (int j = 0; j < _vertexNum; j++)
                if (0==_matrix[i][j] || INT_MAX==_matrix[i][k] || INT_MAX==_matrix[k][j])
                    continue;
                else
                    _matrix[i][j] = std::min(_matrix[i][j], _matrix[i][k]+_matrix[k][j]);
}

/*********************************测试函数*********************************************/
//打印
void test0()
{
    Graph graph;
    graph.printAdjMatrix();
    graph.Floyd_minDistance();
    cout << "After Floyd algorithm:" << endl;
    graph.printAdjMatrix();
}

int main()
{
    test0();
    return 0;
}