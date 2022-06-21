#include <iostream>
#include <fstream>
#include <stack>
#include <list>
#include <vector>
#include <tuple>
#include <bits/stdc++.h>
#include <queue>
#include <cstring>

using namespace std;

void printMatrix (double** X, int m, int n) {
    int i, j;
    for (i=0; i<n; i++) {
        for (j=0; j<n; j++)
            cout << X[i][j] << " ";
        cout << endl;
    };
    cout << endl;
};

class Graph {
    int V, E, *prev;
    double *dist;
    vector<tuple<double, int, int>> edgeList;
        
    public:
        bool* isVisited;
        void clearVisits();
        Graph(int V);
        vector<tuple<double, int>>*  getAdjList();
        double** getAdjMatrix();
        void addEdge (int v, int w, double weight);
        void popEdge(); 
        void printEdges();
        double EdmondsKarp (int source, int sink);
}; 

Graph::Graph (int V) {
    this->V = V, this->E = 0;
    this->isVisited = new bool[V];
    this->prev = new int[V];
    this->dist = new double[V];
    clearVisits();
};

void Graph::clearVisits() {
    for (int i=0; i<V; i++) isVisited[i] = false;
};

void Graph::addEdge (int v, int w, double weight) {
    E++;
    edgeList.push_back (make_tuple(weight, v, w));
};

void Graph::popEdge() {
    E--;
    edgeList.pop_back();
};

void Graph::printEdges() {
    int i;
    cout << "{";
    for (i=0; i<E-1; i++) 
        cout << "(" << get<1>(edgeList[i]) << ", " << get<2>(edgeList[i]) << "), ";
    cout << "(" << get<1>(edgeList[i]) << ", " << get<2>(edgeList[i]) << ")";
    cout << "}" << endl;
};

vector<tuple<double, int>>* Graph::getAdjList() {
    vector<tuple<double, int>>* adjList = new vector<tuple<double, int>>[V]; Graph MST(V);
    for (int i=0; i<E; i++) {
        int v = get<1>(edgeList[i]), w = get<2>(edgeList[i]);
        double weight = get<0>(edgeList[i]);
        adjList[v].push_back(make_tuple(weight, w));
        //adjList[w].push_back(make_tuple(weight, v));
    };
    return adjList;
};

double** Graph::getAdjMatrix() {
    int i, j;
    double** W = new double*[V];
    for (i=0; i<V; i++) W[i] = new double[V];


    for (i=0; i<V; i++) {
        for (j=0; j<V; j++) {
            //if (i == j) W[i][j] = 0;
            W[i][j] = 0;
        };
    };
    for (i=0; i<E; i++) {
        int v = get<1>(edgeList[i]), w = get<2>(edgeList[i]);
        double weight = get<0>(edgeList[i]);
        W[v][w] = weight;
    };
    return W;
};

bool BFS (double** resGraph, int V, int source, int sink, int* prev) {
    int i, j;
    bool* isVisited = new bool[V];
    for (i=0; i<V; i++) isVisited[i] = false;
    queue<int> Q;
    Q.push(source), isVisited[source] = true, prev[source] = -1;

    while (!Q.empty()) {
        i = Q.front();
        Q.pop();

        for (j=0; j<V; j++) {
            if (!isVisited[j] && resGraph[i][j] > 0) {
                if (j == sink) {
                    prev[j] = i;
                    return true;
                };
                Q.push(j), prev[j] = i;
                isVisited[j] = true;
            };
        };
    };
    return false;
};

double Graph::EdmondsKarp (int source, int sink) {
    int u, v;
    double **resGraph = getAdjMatrix(), maxFlow = 0;
    //printMatrix(resGraph, V, V);
    while (BFS(resGraph, V, source, sink, prev)) {
        double pathFlow = INFINITY;
        v = sink;
        while (v != source) {
            u = prev[v];
            pathFlow = min (pathFlow, resGraph[u][v]);
            v = prev[v];
        };
        v = sink;
        while (v != source) {
            u = prev[v];
            resGraph[u][v] -= pathFlow;
            resGraph[v][u] += pathFlow;
            v = prev[v];
        };
        maxFlow += pathFlow;
    };
    //printMatrix(resGraph, V, V);
    return maxFlow;
};

int main (int argc, char** argv) {

    int N, i, j, a, b, k, *W, *L, *R, s, cap;
    double **g, **l; char** names;
    ifstream fin;
    fin.open ("F04/flow.in");
    fin >> N;
    
    names = new char*[N], g = new double*[N], l = new double*[N];
    L = new int[N], W = new int[N], R = new int[N];
    for (i=0; i<N; i++) {
        names[i] = new char [100], g[i] = new double[N], l[i] = new double[N];
        fin >> names[i] >> W[i] >> L[i] >> R[i];
        for (j=0; j<N; j++) fin >> l[i][j];
    };

    for (k=0; k<N; k++) {
        for (i=0, cap=0; i<N; i++) {
            for (j=0; j<N; j++) {
                g[i][j] = l[i][j];
                if (i!=k && j!= k) cap += l[i][j];
            };
        };
        cap /= 2;
        Graph G (N + 1 + (N-1)*(N-2)/2);
        for (i=0, s=0, a=0; i<N; i++) {
            if (i != k) {
                G.addEdge(a+1, N+(N-1)*(N-2)/2, W[k] + R[k] - W[i]);
                for (j=0; j<N; j++) {
                    if (j != k && i != j && g[i][j] > -1) {
                        G.addEdge (0, N+s, g[i][j]);
                        //cout << "connecting " << N+s << "and " << a+1 << endl;
                        G.addEdge (N+s, a+1, INFINITY);
                        if (i<k && k<j) b = a+(j-i);
                        else b = a+(j-i)+1;
                        //cout << "connecting " << N+s << "and " << b << endl;
                        G.addEdge (N+s, b, INFINITY);
                        g[i][j] = -INFINITY, g[j][i] = -INFINITY, s++;
                    };
                };
                a++;
            };
        };
        //cout << G.EdmondsKarp(0, N+(N-1)*(N-2)/2) << " " << cap << endl;
        if (G.EdmondsKarp(0, N+(N-1)*(N-2)/2) < cap) cout << names[k] << " is eliminated." << endl;
    };



    fin.close();
    return 0;
};