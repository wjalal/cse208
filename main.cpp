#include <iostream>
#include <stack>
#include <list>
#include <vector>

using namespace std;

class Graph {

    int V, E;
    bool isDirected, isWeighted;
    vector<int>* adjList;
    void fillVertTopoSort (int v, stack<int>& S);
    stack<int> topoSort();

    public:
        bool* isVisited;
        double* weight;
        void clearVisits();
        Graph (int V, bool isDirected, bool isWeighted);
        void addEdge (int v, int w);
        void addEdge (int v, int w, double weight);
        Graph getTranspose();
        void printDFS (int v);
        void printBFS (int v);
        void printTopoSort();
        void printSCC();
};

void Graph::clearVisits() {
    for (int i=0; i<V; i++) isVisited[i] = false;
};

Graph::Graph (int V, bool isDirected = true, bool isWeighted = false) {
    this->V = V, this->E = 0, this->isDirected = isDirected, this->isWeighted = isWeighted;
    this->adjList = new vector<int>[V], this->isVisited = new bool[V];
    if (isWeighted) this->weight = new double[V];
    clearVisits();
};

void Graph::addEdge (int v, int w) {
    adjList[v].push_back(w);
    if (!isDirected) adjList[w].push_back(v);
};

void Graph::addEdge (int v, int w, double weight) {
    addEdge (v, w);
}

Graph Graph::getTranspose() {
    if (!isDirected) return *this;
    Graph gT(V);
    for (int v=0; v<V; v++)
        for (int w = 0; w < adjList[v].size(); w++) 
            gT.adjList[adjList[v][w]].push_back(v);
    return gT;
};

void Graph::printDFS (int v) {
    isVisited[v] = true;
    cout << v << " ";
    for (int i = 0; i < adjList[v].size(); i++) 
        if (!isVisited[adjList[v][i]]) printDFS (adjList[v][i]);
};

void Graph::printBFS (int v) {
    int u, w; list<int> Q;
    isVisited[v] = true;
    Q.push_back (v);

    while (!Q.empty()) {
        w = Q.front();
        cout << w << " ";
        Q.pop_front();

        for (int i = 0; i < adjList[w].size(); i++) {
            u = adjList[w][i];
            if (!isVisited[u]) {
                isVisited[u] = true;
                Q.push_back (u);
            };
        };
    };
};

void Graph::fillVertTopoSort (int v, stack<int>& S) {
    //clearVisits();
    isVisited[v] = true;
    for (int i = 0; i < adjList[v].size(); i++) 
        if (!isVisited[adjList[v][i]]) fillVertTopoSort (adjList[v][i], S);
    S.push(v);
};

stack<int> Graph::topoSort() {
    stack<int> S;
    clearVisits();
    for (int i=0; i<V; i++) 
        if (!isVisited[i]) fillVertTopoSort(i, S);
    return S; 
};

void Graph::printTopoSort() {
    stack<int> S = topoSort();
    while (!S.empty()) {
        cout << S.top() << " ";
        S.pop();
    };
};

void Graph::printSCC() {
    stack<int> S = topoSort();
    Graph gT = getTranspose();
    //clearVisits();

    while (!S.empty()) {
        int v = S.top();
        S.pop();

        if (!gT.isVisited[v]) {
            gT.printDFS (v);
            cout << endl;
        };
    };
};



int main() {
    // Graph g(4);
    // g.addEdge(0, 1);
    // g.addEdge(0, 2);
    // g.addEdge(1, 2);
    // g.addEdge(2, 0);
    // g.addEdge(2, 3);
    // g.addEdge(3, 3);

    // g.printDFS(2);
    // g.clearVisits();
    // cout << endl;
    // g.printBFS(2);
    // // cout << endl;

    // Graph g(6);
    // g.addEdge(5, 2);
    // g.addEdge(5, 0);
    // g.addEdge(4, 0);
    // g.addEdge(4, 1);
    // g.addEdge(2, 3);
    // g.addEdge(3, 1);
 

    // // Function Call
    // g.printTopoSort();

    Graph g(5);
    g.addEdge(1, 0);
    g.addEdge(0, 2);
    g.addEdge(2, 1);
    g.addEdge(0, 3);
    g.addEdge(3, 4);

    g.printSCC();

    cout << endl;


    return 0;
};
