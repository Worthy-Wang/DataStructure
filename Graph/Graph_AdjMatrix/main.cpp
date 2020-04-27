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
采用邻接矩阵形式创建图(有向图,连通图,加权图)：
vertex : 顶点
EdgeNode: 边节点
*/

using ElemType = char;

/*********************************类申明*********************************************/
class Graph
{
    vector<vector<int>> _matrix; //邻接矩阵
    vector<ElemType> _vals;      //存放顶点元素
    int _vertexNum;              //顶点数量
    int _edgeNum;                //边的数量
    vector<int> visited;         //判断定点是否访问过

public:
    Graph();
    void DFS(int);              //DFS 深度优先遍历
    void BFS(int);              //BFS 广度优先遍历
    void DFS_notRecursion(int); //DFS 深度优先遍历非递归


    void printAdjMatrix(); //测试邻接矩阵是否正确组合

    int firstNeighbor(int i)
    {
        if (i >= 0 && i < _vertexNum - 1)
        {
            for (int j = 0; j < _vertexNum; j++)
                if (_matrix[i][j] > 0 && _matrix[i][j] < INT_MAX)
                    return j;
        }
        return -1;
    }
    int nextNeighbor(int i, int n)
    {
        for (int j = n + 1; j < _vertexNum; j++)
            if (_matrix[i][j] > 0 && _matrix[i][j] < INT_MAX)
                return j;
        return -1;
    }

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
            _matrix[i][j] = weight; //有向加权图
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

void Graph::DFS(int root)
{
    cout << _vals[root] << " -> ";
    visited[root] = 1;
    int neighbor = firstNeighbor(root); // 得到firstChild
    while (neighbor != -1)
    {
        if (!visited[neighbor])
            DFS(neighbor);
        else
            neighbor = nextNeighbor(root, neighbor);
    }
}

void Graph::BFS(int index)
{
    std::queue<int> que;
    que.push(index);
    visited[index] = 1;
    cout << _vals[index] << " -> ";

    while (!que.empty())
    {
        int root = que.front();
        que.pop();
        int neighbor = firstNeighbor(root);
        while (neighbor != -1)
        {
            if (!visited[neighbor])
            {
                que.push(neighbor);
                visited[neighbor] = 1;
                cout << _vals[neighbor] << " -> ";
            }
            neighbor = nextNeighbor(root, neighbor);
        }
    }
}

//非递归DFS
void Graph::DFS_notRecursion(int index)
{
    std::stack<int> mystack; //只存放顶点

    //根节点
    cout << _vals[index] << " -> ";
    mystack.push(index);
    visited[index] = 1;
    int root = index;
    int neighbor = firstNeighbor(root);

    while (1)
    {
        //向左
        while (neighbor != -1)
        {
            if (!visited[neighbor])
            {
                cout << _vals[neighbor] << " -> ";
                mystack.push(neighbor);
                visited[neighbor] = 1;
            }
            root = neighbor;
            neighbor = firstNeighbor(root);
        }

        //向右
        if (!mystack.empty())
        {
            root = mystack.top();
            neighbor = firstNeighbor(root);
            while (neighbor != -1)
            {
                if (!visited[neighbor])
                    break;
                neighbor = nextNeighbor(root, neighbor);
            }
            if (neighbor == -1)
                mystack.pop();
        }
        else
        {
            break;
        }
    }
}

/*********************************测试函数*********************************************/
void test1()
{
    Graph graph;
    graph.printAdjMatrix();
    cout << graph.firstNeighbor(0) << endl;
    cout << graph.nextNeighbor(0, 1) << endl;
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

int main()
{
    test1();
    return 0;
}