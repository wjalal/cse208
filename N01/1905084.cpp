#include <iostream>
#include <stack>
#include <list>
#include <vector>
#include <cstring>

using namespace std;

class Graph {

    int V, E, *pred, *dist;
    bool isDirected;
    vector<int>* adjList;
    void fillVertTopoSort (int v, stack<int>& S);
    stack<int> topoSort();

    public:
        bool* isVisited;
        void clearVisits();
        Graph (int V);
        void addEdge (int v, int w);

        bool distBFS (int src, int dest, int v ) {
            list<int> Q;
            clearVisits();
            for (int i=0; i<V; i++) {
                pred[i] = -1;
                dist[i] = INT32_MAX;
            };
            isVisited[src] = true;
            dist[src] = 0;
            Q.push_back(src);

            while (!Q.empty()) {
                int u = Q.front();
                Q.pop_front();
                for (int i = 0; i < adjList[u].size(); i++) {
                    if (!isVisited[adjList[u][i]]) {
                        isVisited[adjList[u][i]] = true;
                        dist[adjList[u][i]] = dist[u] + 1;
                        pred[adjList[u][i]] = u;
                        Q.push_back(adjList[u][i]);
                        if (adjList[u][i] == dest) return true;
                    };
                };
            };

            return false;
        };

        void printDist(int src, int dest, int v ) {
            int pred[v], dist[v];

            if (!distBFS(src, dest, v)) return;

            cout << dist[dest];
        };
};

void Graph::clearVisits() {
    for (int i=0; i<V; i++) isVisited[i] = false;
};

Graph::Graph (int V) {
    this->V = V;
    this->adjList = new vector<int>[V];
    this->pred = new int[V];
    this->dist = new int[V];
    this->isDirected = true;
    this->isVisited = new bool[V];
    clearVisits();
};

void Graph::addEdge (int v, int w) {
    adjList[v].push_back(w);
    if (!isDirected) adjList[w].push_back(v);
};


int main() {

    int n, i, j;
    char str[100];
    cin >> str;

    n = strlen(str) - 1;

    Graph g(n);
    for (i=0; i<n; i++) {
        if (i>0) g.addEdge(i, i-1);
        if (i<n-1) g.addEdge(i, i+1);
        for (j=0; j<n; j++) {
            if (j!=i && str[j]==str[i]) g.addEdge(i,j);
        };
    };

    g.printDist(0, n-1, n);

    cout << endl;


    return 0;
};
