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
AOV网与AOE网：
AOV(activity on vertex) 网：在执行后继节点之前，必须将所有指向后继节点的前驱节点都完成
AOE(activity on edge) 网：在执行后继边的活动之前，必须将前驱边的活动都完成

拓扑排序：拓扑排序时判断有环无环的算法
依次找到入度为0的顶点，打印，并从图中消去该顶点和弧，如果消去的顶点数==总顶点数，则无环；反之有环


关键路径：

ve:事件最早发生时间，最早也要等到前序所有的工作做完 （正向加 取权值最大）
vl:事件最晚发生时间，最晚也要等到能保证完成后面的工作 （逆向见减 取权值最小）
ae:活动最早发生事件，等于 头顶点的的Ve值
al:活动最晚发生时间，等于 尾顶点的Vl值减去权值
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
    vector<EdgeNode *> _edges;                  //存放所有边的信息
    map<int, set<EdgeNode *, Comp>> _vertexMap; //存放相应顶点的边信息
    vector<int> _inDegree;                      //存放每一个顶点的入度
    int _vertexNum;                             //顶点数量
    int _edgeNum;                               //边的数量
    vector<int> visited;                        //判断定点是否访问过

public:
    Graph();
    void printAdjMatrix();      //测试邻接矩阵是否正确组合
    void printvertexAndEdges(); //测试顶点及其相邻边是否正确
    bool topologicalSort();     //进行拓扑排序，成功返回true, 失败返回false（有环）
    void criticalPath();        //找寻关键路径

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
        _inDegree.resize(_vertexNum);
        for (int k = 0; k < _edgeNum; k++)
        {
            getline(ifs, str);
            stringstream ss(str);
            int i, j, weight; //用边连接的两个顶点的编号
            ss >> i >> j >> weight;
            _matrix[i][j] = weight;

            //有向加权图
            EdgeNode *edge = new EdgeNode(i, j, weight);
            _inDegree[j]++;
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

void Graph::printvertexAndEdges()
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

bool Graph::topologicalSort()
{
    resetVisited();
    stack<int> s;
    int cnt = 0;
    //找出入度为0的顶点
    for (int i = 0; i < _vertexNum; i++)
    {
        if (!visited[i] && 0 == _inDegree[i])
        {
            s.push(i);
            visited[i] = 1;
            cnt++;
        }
    }

    cout << "拓扑排序：";
    while (!s.empty())
    {
        //消去所有入度为0的顶点的所有弧
        while (!s.empty())
        {
            int v = s.top();
            cout << _vals[v] << "->";
            s.pop();
            for (int j = 0; j < _vertexNum; j++)
            {
                if (_matrix[v][j] > 0 && _matrix[v][j] < INT_MAX)
                    _inDegree[j]--;
            }
        }

        //找出入度为0的顶点
        for (int i = 0; i < _vertexNum; i++)
        {
            if (!visited[i] && 0 == _inDegree[i])
            {
                s.push(i);
                visited[i] = 1;
                cnt++;
            }
        }
    }
    cout << endl;

    return (cnt == _vertexNum) ? true : false;
}

void Graph::criticalPath()
{
    vector<int> ve(_vertexNum, 0);
    vector<int> vl(_vertexNum, INT_MAX);
    vector<int> ae(_edges.size());
    vector<int> al(_edges.size());
    vector<pair<int,int>> path;
    //计算ve
    for (int i = 0; i < _vertexNum; i++)
    {
        for (int j = 0; j < _vertexNum; j++)
        {
            if (_matrix[i][j] > 0 && _matrix[i][j] < INT_MAX)
                if (ve[i] + _matrix[i][j] > ve[j])
                    ve[j] = ve[i] + _matrix[i][j];
        }
    }
    //vl
    vl[_vertexNum - 1] = ve[_vertexNum - 1];
    for (int j = _vertexNum - 1; j >= 0; j--)
    {
        for (int i = 0; i < _vertexNum; i++)
        {
            if (_matrix[i][j] > 0 && _matrix[i][j] < INT_MAX)
                if (vl[i] > vl[j] - _matrix[i][j])
                    vl[i] = vl[j] - _matrix[i][j];
        }
    }

    //ae
    int edgeSize = _edges.size();
    int cnt = 0;
    for (int i = 0; i < _vertexNum; i++)
        for (int j = 0; j < _vertexNum; j++)
            if (_matrix[i][j] > 0 && _matrix[i][j] < INT_MAX)
                ae[cnt++] = ve[i];

    //al
    cnt = 0;
    for (int i = 0; i < _vertexNum; i++)
        for (int j = 0; j < _vertexNum; j++)
            if (_matrix[i][j] > 0 && _matrix[i][j] < INT_MAX)
            {
                al[cnt++] = vl[j] - _matrix[i][j];
                if (al[cnt-1] == ae[cnt-1])
                    path.push_back(std::make_pair(i,j));
            }

    //print
    cout << "ve:" << endl;
    for (int i = 0; i < _vertexNum; i++)
        cout << ve[i] << " ";
    cout << endl;

    cout << "vl:" << endl;
    for (int i = 0; i < _vertexNum; i++)
        cout << vl[i] << " ";
    cout << endl;

    cout << "ae:" << endl;
    for (int i = 0; i < edgeSize; i++)
        cout << ae[i] << " ";
    cout << endl;

    cout << "al:" << endl;
    for (int i = 0; i < edgeSize; i++)
        cout << al[i] << " ";
    cout << endl;

    cout << "CriticalPath:" << endl;
    for (int i = 0; i < path.size(); i++)
        cout << _vals[path[i].first] << "->" << _vals[path[i].second] << " ";
    cout << endl;
}

/*********************************测试函数*********************************************/
//TopologicalSort 判断是否为DAG并打印拓扑序列
void test0()
{
    Graph graph;
    graph.printAdjMatrix();
    graph.printvertexAndEdges();
    (graph.topologicalSort() == true) ? (cout << "无环" << endl) : (cout << "有环" << endl);
}

//Criticial_path
void test1()
{
    Graph graph;
    graph.printAdjMatrix();
    graph.printvertexAndEdges();
    graph.criticalPath();
}

int main()
{
    test1();
    return 0;
}