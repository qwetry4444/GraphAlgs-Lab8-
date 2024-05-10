//#include <values>
#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <float.h>
#include <vector>
#include <limits>
#include <queue>
#include <stack>
#include <set>

//В случае если код, представленный в данном файле, не запускается, то в зависимости от вида возникших ошибок
//необходимо выполнить одно или несколько из следующих действий:
//неизвестна iostream.h: убрать расширение h в строке #include <iostream.h>;
//values.h неизвестна компилятору: закомментировать строку #include <values.h>
//неизвестна константа DBL_MAX: добавить #include <float.h> и заменить DBL_MAX на DBL_MAX во всем коде данного файла

//---------------------------------------------------------------------------
struct Item {
    int D;
    Item* Next;
    Item() :D(-1), Next(0) {}
    Item(int V) : D(V), Next(0) {}
};
//---------------------------------------------------------------------------
class Cont {
protected:
    Item* _Head;
    Item* _Tail;
public:
    Cont() :_Head(0), _Tail(0) {}
    bool Empty(void) { return _Head ? false : true; }
    virtual void Push(int V) = 0;
    virtual int  Pop(void);
    ~Cont();
};
//---------------------------------------------------------------------------
class CStack : public Cont {
public:
    CStack() :Cont() {}
    virtual void Push(int V);
};
//---------------------------------------------------------------------------
class CQueue : public Cont {
public:
    CQueue() :Cont() {}
    virtual void Push(int V);
};
//---------------------------------------------------------------------------
class Matrix {
protected:
    double** _m;
    int _Size;
    virtual  void   Init(double Value);
public:
    Matrix(int Size);
    Matrix(const Matrix& M);
    double operator () (int row, int col);
    int    Size(void) { return _Size; }
    void   Set(int row, int col, double Value);
    virtual  void   Print(void);
    void   PrintFloydPath(int From, int To);
    void   PrintFloydPaths();
    ~Matrix();
};
//---------------------------------------------------------------------------
class Graph : public Matrix {
    virtual void    Init(double Value);
public:
    Graph(int Size) :Matrix(Size) { Init(DBL_MAX); }
    Graph(const Graph& G) :Matrix(G) {}
    virtual void    Random(double Density, double MaxWeight = 1);
    virtual void    Print(void);
    virtual void    AddEdge(int V0, int V1, double Weight = 1);
    virtual void    DeleteEdge(int V0, int V1);
    virtual  int    EdgeCount(void);
    int    VertexCount(void) { return _Size; }
    virtual Graph   ShortestPath(int From, int To);
    void    Visit(int From, Cont& C);
    void    FindMinEdge(int& V0, int& V1);
    Graph   Floyd(Matrix& M);
    Graph   Kruskal(void);
    int     Hamiltonian(int v, int w, int Length, bool* Labelled, Graph& G);
    Graph   HamiltonianPath(int From, int To);

    std::vector<double> dijkstraAvoid(int From, int Avoid);

    bool IsConnected();

    void GenerateEulerGraph();
    bool IsEulerian();
    std::vector<int> FindEulerCycle();

    void GenerateHamiltonianGraph();
    bool IsHamiltonian();
    bool FindHamiltonianCycle(std::vector<int>& path, std::vector<bool>& visited, int current);
};
//---------------------------------------------------------------------------
class SGraph : public Graph {
public:
    SGraph(int Size) :Graph(Size) {}
    SGraph(const SGraph& G) : Graph(G) {}
    virtual void    Random(double Density, double MaxWeight = 1);
    virtual  int    EdgeCount(void) { return Graph::EdgeCount() / 2; };
    virtual void    AddEdge(int V0, int V1, double Weight = 1);
};
//---------------------------------------------------------------------------
class WGraph : public Graph {
public:
    WGraph(int Size) :Graph(Size) {}
    WGraph(const WGraph& G) : Graph(G) {}
    virtual  int    EdgeCount(void) { return Graph::EdgeCount() / 2; };
    virtual void    AddEdge(int V0, int V1, double Weight = 1);
};
//---------------------------------------------------------------------------
class OrGraph : public Graph {
public:
    OrGraph(int Size) :Graph(Size) {}
    OrGraph(const OrGraph& G) : Graph(G) {}
    virtual void    Random(double Density, double MaxWeight = 1);
    virtual void    AddEdge(int V0, int V1, double Weight = 1);
    OrGraph TransClosure(bool PathOrder);

};
//---------------------------------------------------------------------------
class OrWGraph : public Graph {
public:
    OrWGraph(int Size) :Graph(Size) {}
    OrWGraph(const OrWGraph& G) : Graph(G) {}
    virtual void    Random(double Density, double MaxWeight = 1);
};
//---------------------------------------------------------------------------
struct Deikstra {
    bool   Label;
    double Path;
    int    Vertex;
    Deikstra() :Label(false), Path(DBL_MAX), Vertex(-1) {}
};
//---------------------------------------------------------------------------
int Cont::Pop(void)
{
    if (_Head)
    {
        Item* I = _Head;
        _Head = _Head->Next;
        int V = I->D;
        delete I;
        if (!_Head) _Tail = 0;
        return V;
    }
    return -1;
}
//---------------------------------------------------------------------------
Cont::~Cont()
{
    while (_Head)
    {
        Item* I = _Head->Next;
        delete _Head;
        _Head = I;
    }
}
//---------------------------------------------------------------------------
void CStack::Push(int V)
{
    if (_Head)
    {
        Item* I = new Item(V);
        I->Next = _Head;
        _Head = I;
    }
    else
        _Tail = _Head = new Item(V);
}
//---------------------------------------------------------------------------
void CQueue::Push(int V)
{
    if (_Head)
    {
        _Tail->Next = new Item(V);
        _Tail = _Tail->Next;
    }
    else
        _Tail = _Head = new Item(V);
}
//---------------------------------------------------------------------------
Matrix::Matrix(int Size) :_Size(0), _m(0)
{
    if (Size > 0)
    {
        _Size = Size;
        _m = new double* [_Size];
        for (int i = 0; i < _Size; i++)
            _m[i] = new double[_Size];
        Init(0);
    }
}
//---------------------------------------------------------------------------
Matrix::Matrix(const Matrix& M)
{
    if (&M != this)
    {
        _Size = 0;
        _m = 0;
        if (M._Size > 0)
        {
            _Size = M._Size;
            _m = new double* [_Size];
            for (int i = 0; i < _Size; i++)
                _m[i] = new double[_Size];
        }
        for (int i = 0; i < _Size; i++)
            for (int j = 0; j < _Size; j++)
                _m[i][j] = M._m[i][j];
    }
}
//---------------------------------------------------------------------------
void   Matrix::Init(double Value)
{
    for (int i = 0; i < _Size; i++)
        for (int j = 0; j < _Size; j++)
            _m[i][j] = Value;
}
//---------------------------------------------------------------------------
double Matrix::operator () (int row, int col)
{
    if (row < _Size && col < _Size && row >= 0 && col >= 0)
        return _m[row][col];
    return DBL_MAX;
}
//---------------------------------------------------------------------------
void   Matrix::Set(int row, int col, double Value)
{
    if (row < _Size && col < _Size && row >= 0 && col >= 0)
        _m[row][col] = Value;
}
//---------------------------------------------------------------------------
void   Matrix::Print(void)
{
    for (int i = 0; i < _Size; i++)
    {
        for (int j = 0; j < _Size; j++)
            printf("%10.2lf", _m[i][j]);
        printf("\n");
    }
}
//---------------------------------------------------------------------------
void   Matrix::PrintFloydPath(int From, int To)
{
    if (From != To)
    {
        printf("Path from %d to %d :  ", From, To);
        int v = From;
        printf("%d ", v);
        do
        {
            v = _m[v][To];
            if (v == -1)
            {
                printf("--> |x|");
                break;
            }
            printf("--> %d ", v);
        } while (v != To);
        printf("\n");
    }
}
//---------------------------------------------------------------------------
void   Matrix::PrintFloydPaths()
{
    for (int v = 0; v < _Size; v++)
        for (int w = 0; w < _Size; w++)
            PrintFloydPath(v, w);
    printf("\n");
}
//---------------------------------------------------------------------------
Matrix::~Matrix()
{
    for (int i = 0; i < _Size; i++)
        delete[] _m[i];
    delete[] _m;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void   Graph::Init(double Value)
{
    for (int i = 0; i < _Size; i++)
        for (int j = 0; j < _Size; j++)
            if (i != j) _m[i][j] = Value;
}
//---------------------------------------------------------------------------
void    Graph::AddEdge(int V0, int V1, double Weight)
{
    if (V0 >= 0 && V1 >= 0 && V0 < _Size && V1 < _Size && V0 != V1 && Weight >= 0)
    {
        _m[V0][V1] = Weight;
    }
}
//---------------------------------------------------------------------------
void    Graph::DeleteEdge(int V0, int V1)
{
    if (V0 >= 0 && V1 >= 0 && V0 < _Size && V1 < _Size && V0 != V1)
    {
        _m[V0][V1] = DBL_MAX;
    }
}
//---------------------------------------------------------------------------
void   Graph::Random(double Density, double MaxWeight)
{
    if (Density >= 0 && Density <= 1 && MaxWeight >= 0)
    {
        for (int i = 0; i < _Size; i++)
            for (int j = 0; j < _Size; j++)
                if (i > j)
                {
                    if (Density >= (double)rand() / RAND_MAX)
                        _m[i][j] = MaxWeight * rand() / RAND_MAX;
                    else
                        _m[i][j] = DBL_MAX;
                    _m[j][i] = _m[i][j];
                }
                else
                    continue;
    }
}
//---------------------------------------------------------------------------
void   Graph::Print(void)
{
    for (int i = 0; i < _Size; i++)
    {
        for (int j = 0; j < _Size; j++)
            if (_m[i][j] < DBL_MAX)
                printf("%7.1lf", _m[i][j]);
            else
                printf("     oo");
        printf("\n");
    }
    printf("\n");
}
//---------------------------------------------------------------------------
Graph Graph::ShortestPath(int From, int To)
{
    if (From >= 0 && From < _Size && To < _Size && To >= 0)
    {
        if (From == To) return Graph(0);
        Deikstra* D = new Deikstra[_Size];
        D[From].Path = 0;
        D[From].Vertex = -1;
        int U, V = From;
        double SPath;
        do
        {
            D[V].Label = true;
            for (U = 0; U < _Size; U++)
            {
                if (D[U].Label || _m[V][U] == DBL_MAX) continue;
                if (D[U].Path > D[V].Path + _m[V][U])
                {
                    D[U].Path = D[V].Path + _m[V][U];
                    D[U].Vertex = V;
                }
            }
            SPath = DBL_MAX;
            V = -1;
            for (U = 0; U < _Size; U++)
                if (!(D[U].Label) && D[U].Path < SPath)
                {
                    SPath = D[U].Path;
                    V = U;
                }
        } while (V != -1 && V != To);
        Graph G(_Size);
        V = To;
        while (D[V].Vertex != -1)
        {
            G.AddEdge(V, D[V].Vertex, _m[V][D[V].Vertex]);
            V = D[V].Vertex;
        }
        delete[] D;
        return G;
    }
    return Graph(0);
}
//---------------------------------------------------------------------------
int    Graph::EdgeCount(void)
{
    if (_Size)
    {
        int Count = 0;
        for (int i = 0; i < _Size; i++)
            for (int j = 0; j < _Size; j++)
            {
                if (i == j) continue;
                if (_m[i][j] < DBL_MAX) Count++;
            }
        return Count;
    }
    return 0;
}
//---------------------------------------------------------------------------
Graph   Graph::Floyd(Matrix& M)
{
    Graph G(*this);
    if (M.Size() == _Size)
    {
        for (int i = 0; i < _Size; i++)
            for (int s = 0; s < _Size; s++)
                M.Set(i, s, G._m[i][s] == DBL_MAX ? -1 : s);
        for (int i = 0; i < _Size; i++)
            for (int s = 0; s < _Size; s++)
                if (G._m[s][i] < DBL_MAX)
                    for (int t = 0; t < _Size; t++)
                        if (G._m[i][t] < DBL_MAX)
                            if (G._m[s][t] > G._m[s][i] + G._m[i][t])
                            {
                                G._m[s][t] = G._m[s][i] + G._m[i][t];
                                M.Set(s, t, M(s, i));
                            }
    }
    return G;
}
//---------------------------------------------------------------------------
void    Graph::FindMinEdge(int& V0, int& V1)
{
    double MinWeight = DBL_MAX;
    V0 = V1 = -1;
    for (int i = 0; i < _Size; i++)
        for (int s = 0; s < _Size; s++)
        {
            if (i == s)  continue;
            if (_m[i][s] < DBL_MAX)
            {
                MinWeight = _m[i][s];
                V0 = i;
                V1 = s;
            }
        }
    if (MinWeight == DBL_MAX)
        V0 = V1 = -1;
}
//---------------------------------------------------------------------------
Graph   Graph::Kruskal(void)
{
    Graph MST(_Size);
    Graph G(*this);
    bool* Labelled = new bool[_Size];
    for (int i = 0; i < _Size; i++) Labelled[i] = false;
    int V0 = -1, V1 = -1;
    while (MST.EdgeCount() < (_Size - 1) * 2)
    {
        G.FindMinEdge(V0, V1);
        if (V0 == -1 && V1 == -1)
            break;
        if (!Labelled[V0] || !Labelled[V1])
        {
            MST.AddEdge(V0, V1, _m[V0][V1]);
            MST.AddEdge(V1, V0, _m[V0][V1]);
            Labelled[V0] = true;
            Labelled[V1] = true;
        }
        G.DeleteEdge(V0, V1);
        G.DeleteEdge(V1, V0);
    }
    delete[] Labelled;
    return MST;
}
//---------------------------------------------------------------------------
void    Graph::Visit(int From, Cont& C)
{
    if (From >= 0 && From < _Size)
    {
        int V = From;
        bool* Labelled = new bool[_Size];
        for (int i = 0; i < _Size; i++)
            Labelled[i] = false;
        C.Push(V);
        Labelled[V] = true;
        do
        {
            V = C.Pop();
            printf("%d ", V);
            for (int U = 0; U < _Size; U++)
            {
                if (!Labelled[U] && _m[V][U] != DBL_MAX)
                {
                    C.Push(U);
                    Labelled[U] = true;
                }
            }
        } while (!C.Empty());
        delete[] Labelled;
        printf("\n");
    }
}
//---------------------------------------------------------------------------
int  Graph::Hamiltonian(int v, int w, int Length, bool* Labelled, Graph& G)
{
    if (v == w)
        return Length == 0 ? 1 : 0;
    Labelled[v] = true;
    for (int t = 0; t < _Size; t++)
        if (_m[v][t] < DBL_MAX && _m[v][t] != 0 && !Labelled[t])
            if (Hamiltonian(t, w, Length - 1, Labelled, G))
            {
                G.AddEdge(t, v, _m[t][v]);
                G.AddEdge(v, t, _m[v][t]);
                return 1;
            }
    Labelled[v] = false;
    return 0;
}
//---------------------------------------------------------------------------
Graph   Graph::HamiltonianPath(int From, int To)
{
    bool* Labelled = new bool[_Size];
    for (int i = 0; i < _Size; i++) Labelled[i] = false;
    Graph G(_Size);
    int f = Hamiltonian(From, To, _Size - 1, Labelled, G);
    delete[] Labelled;
    return G;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void    SGraph::AddEdge(int V0, int V1, double Weight)
{
    Graph::AddEdge(V0, V1, 1);
    Graph::AddEdge(V1, V0, 1);
}
//---------------------------------------------------------------------------
void   SGraph::Random(double Density, double MaxWeight)
{
    if (Density >= 0 && Density <= 1 && MaxWeight >= 0)
    {
        for (int i = 0; i < _Size; i++)
            for (int j = 0; j < _Size; j++)
                if (i > j)
                {
                    if (Density >= (double)rand() / RAND_MAX)
                        _m[i][j] = 1.0;
                    else
                        _m[i][j] = DBL_MAX;
                    _m[j][i] = _m[i][j];
                }
                else
                    continue;
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void    WGraph::AddEdge(int V0, int V1, double Weight)
{
    Graph::AddEdge(V0, V1, Weight);
    Graph::AddEdge(V1, V0, Weight);
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void    OrGraph::AddEdge(int V0, int V1, double Weight)
{
    Graph::AddEdge(V0, V1, 1);
}
//---------------------------------------------------------------------------
OrGraph OrGraph::TransClosure(bool PathOrder)
{
    OrGraph G(*this);
    for (int i = 0; i < _Size; i++)
        for (int s = 0; s < _Size; s++)
            if (G._m[s][i] == 1)
                for (int t = 0; t < _Size; t++)
                    G._m[s][t] = G._m[i][t] == 1 ? 1 : G._m[s][t];
    if (!PathOrder)
        for (int i = 0; i < _Size; i++)
            G._m[i][i] = 0;
    return G;
}
//---------------------------------------------------------------------------
void   OrGraph::Random(double Density, double MaxWeight)
{
    if (Density >= 0 && Density <= 1 && MaxWeight >= 0)
    {
        for (int i = 0; i < _Size; i++)
            for (int j = 0; j < _Size; j++)
            {
                if (i == j) continue;
                if (Density >= (double)rand() / RAND_MAX)
                    _m[i][j] = 1.0;
                else
                    _m[i][j] = DBL_MAX;
            }
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void   OrWGraph::Random(double Density, double MaxWeight)
{
    if (Density >= 0 && Density <= 1 && MaxWeight >= 0)
    {
        for (int i = 0; i < _Size; i++)
            for (int j = 0; j < _Size; j++)
            {
                if (i == j) continue;
                if (Density >= (double)rand() / RAND_MAX)
                    _m[i][j] = MaxWeight * rand() / RAND_MAX;
                else
                    _m[i][j] = DBL_MAX;
            }
    }
}


std::vector<double> Graph::dijkstraAvoid(int start, int avoid) {
    int n = Size();
    std::vector<double> distance(n, std::numeric_limits<double>::infinity());
    std::vector<bool> visited(n, false);
    std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> pq;

    distance[start] = 0;
    pq.push({ 0, start });

    while (!pq.empty()) {
        int u = pq.top().second;
        pq.pop();

        if (visited[u])
            continue;

        visited[u] = true;

        for (int v = 0; v < n; ++v) {
            if (u == avoid || v == avoid)
                continue;

            if (_m[u][v] != std::numeric_limits<double>::infinity()) {
                double weight = _m[u][v];
                if (distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                    pq.push({ distance[v], v });
                }
            }
        }
    }

    return distance;
}



bool Graph::IsConnected() {
    std::vector<bool> visited(_Size, false);
    std::queue<int> q;
    int countVisited = 0;
    q.push(0); 

    while (!q.empty()) {
        int vertex = q.front();
        q.pop();
        if (visited[vertex] == false)
        {
            visited[vertex] = true;
            countVisited++;
            for (int neighbor = 0; neighbor < _Size; neighbor++) {
                if (_m[vertex][neighbor] != DBL_MAX && !visited[neighbor]) {
                    q.push(neighbor);
                }
            }
        }
    }
    return countVisited == _Size;
}

template<typename S>
auto select_random(const S& s, size_t n) {
    auto it = std::begin(s);
    std::advance(it, n);
    return it;
}


void Graph::GenerateEulerGraph()
{
    int startVertex = rand() % _Size;
    int currentVertex = startVertex;
    int nextVertex;
    std::vector<int> degrees(_Size, 2);

    std::set<int> nodes;
    for (int i = 0; i < _Size; i++) {
        nodes.insert(i);
    }

    while (true)
    {
        int rNext = rand() % nodes.size();
        int rCurrent = rand() % nodes.size();
        nextVertex = *select_random(nodes, rNext);
        currentVertex = *select_random(nodes, rCurrent);


        if (degrees[nextVertex] != 0 && degrees[currentVertex] != 0 && nextVertex != currentVertex)
        {
            AddEdge(nextVertex, currentVertex);
            AddEdge(currentVertex, nextVertex);

            degrees[nextVertex]--;
            degrees[currentVertex]--;

            if (degrees[nextVertex] == 0)
                nodes.erase(nextVertex);
            if (degrees[currentVertex] == 0)
                nodes.erase(currentVertex);

            currentVertex = nextVertex;
        }
        if (nodes.size() < 2)
        {
            break;
        }
    }
    
    for (int i = 0; i < _Size; i++) {
        for (int j = 0; j < _Size; j++) {
            if (_m[i][j] == DBL_MAX) {
                AddEdge(i, j);
            }
        }
    }
}



bool Graph::IsEulerian() {
    if (!IsConnected()) {
        std::cout << "Граф не является связным.\n";
        return false;
    }

    int oddDegreeVertices = 0;
    for (int i = 0; i < _Size; i++) {
        int degree = -1;
        for (int j = 0; j < _Size; j++) {
            if (_m[i][j] != DBL_MAX) {
                degree++;
            }
        }
        if (degree % 2 != 0) {
            oddDegreeVertices++;
        }
    }

    if (oddDegreeVertices > 2) {
        std::cout << "Граф имеет более двух вершин с нечетной степенью.\n";
        return false;
    }

    return oddDegreeVertices == 0 || oddDegreeVertices == 2;
}

std::vector<int> Graph::FindEulerCycle()
{
    int currentVertex;
    bool found_edge = false;
    int startVertex = 0;

    std::stack<int> stack;
    std::vector<int> path(0);

    //if (!IsEulerian()) {
    //    std::cout << "Граф не содержит Эйлеров цикл" << std::endl;
    //    return path;
    //}


    for (int i = 0; i < _Size; i++) {
        int degree = 0;
        for (int neighbor = 0; neighbor < _Size; neighbor++) {
            if (_m[i][neighbor] != DBL_MAX && i != neighbor) {
                degree++;
            }
        }
        if (degree % 2 == 1) {
            startVertex = i;
            break;
        }
    }


    stack.push(startVertex);
    while (!stack.empty())
    {
        currentVertex = stack.top();
        found_edge = false;
        for (int neighbor = 0; neighbor < _Size; neighbor++) {
            if (_m[currentVertex][neighbor] != DBL_MAX
                && currentVertex != neighbor)
            {
                stack.push(neighbor);
                found_edge = true;
                _m[currentVertex][neighbor] = DBL_MAX;
                _m[neighbor][currentVertex] = DBL_MAX;
                break;
            }
        }
        if (!found_edge)
        {
            stack.pop();
            path.push_back(currentVertex);
        }
    }

    //for (int i = 0; i < path.size(); i++) {
        //std::cout << path[i] << " -> ";
    //}

    return path;
}


void Graph::GenerateHamiltonianGraph()
{
    int startVertex = rand() % _Size;

    for (int i = 0; i < _Size; i++) {
        for (int j = 0; j < _Size; j++) {
            if (i != j && _m[i][j] == DBL_MAX) {
                if (rand() % 4 == 1) {
                    AddEdge(i, j);
                    AddEdge(j, i);
                }
            }
        }
    }

    int degree;
    for (int i = 0; i < _Size; i++) {
        degree = 0;
        for (int j = 0; j < _Size; j++) {
            if (_m[i][j] != DBL_MAX && i != j) {
                degree++;
            }
        }
        while (degree < _Size / 2.0) {
            int randVertex = rand() % _Size;
            if (randVertex != i && _m[i][randVertex] == DBL_MAX) {
                AddEdge(i, randVertex);
                AddEdge(randVertex, i);
                degree++;
            }
        }
    }
}



bool Graph::IsHamiltonian()
{
    std::vector<int> degrees(_Size, 0);
    for (int i = 0; i < _Size; i++) {
        for (int j = 0; j < _Size; j++) {
            if (i != j && _m[i][j] != DBL_MAX) {
                degrees[i]++;
            }
        }
        degrees[i] /= 2;
    }
    for (int i = 0; i < _Size; i++) {
        if (degrees[i] < _Size / 2) {
            return false;
        }
    }
    return true;
}

bool Graph::FindHamiltonianCycle(std::vector<int>& path, std::vector<bool>& visited, int current) 
{
    path.push_back(current);
    if (path.size() == _Size) {
        if (_m[path[0]][path.back()] != DBL_MAX) {
            return true;
        }
        else {
            path.pop_back();
            return false;
        }
    }
    visited[current] = true;

    for (int next = 0; next < _Size; next++) {
        if (_m[current][next] != DBL_MAX && !visited[next]) {
            if (FindHamiltonianCycle(path, visited, next)) {
                return true;
            }
        }
    }
    visited[current] = false;
    path.pop_back();
    return false;
}

void euler_time_analyze(int vCount) 
{
    Graph graph(vCount);
    graph.GenerateEulerGraph();
    
    clock_t start = clock();

    graph.FindEulerCycle();

    clock_t end = clock();
    std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
}

void hamiltonian_time_analyze(int vCount)
{
    Graph graph(vCount);
    graph.GenerateHamiltonianGraph();

    clock_t start = clock();

    std::vector<int> path(0);
    std::vector<bool> visited(graph.Size());
    graph.FindHamiltonianCycle(path, visited, 0);

    clock_t end = clock();
    std::cout << (double)(end - start) / CLOCKS_PER_SEC << std::endl;
}


#pragma argsused
//---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
    system("chcp 1251");


    //Graph graph = Graph(5);
    //graph.GenerateEulerGraph();
    //
    //graph.Print();
    //if (graph.IsEulerian()) {
    //    std::cout << "Граф содержит эйлеров цикл" << std::endl;
    //}
    //else {
    //    std::cout << "Граф не содержит эйлеров цикл" << std::endl;
    //}
    //graph.FindEulerCycle();

    //graph.GenerateHamiltonianGraph();

    //graph.Print();
    //if (graph.IsHamiltonian()) {
    //    std::cout << "Граф содержит гамильтонов цикл" << std::endl;
    //}
    //else {
    //    std::cout << "Граф не содержит гамильтонов цикл" << std::endl;
    //}

    //std::vector<int> path(0);
    //std::vector<bool> visited(graph.Size());

    //graph.FindHamiltonianCycle(path, visited, 0);
    //for (int i = 0; i < path.size(); i++) {
    //    std::cout << path[i] << " -> ";
    //}



    for (int i = 101; i < 2000; i += 100) {
        //euler_time_analyze(i);
        hamiltonian_time_analyze(i);
    }

    return 0;
}
//---------------------------------------------------------------------------