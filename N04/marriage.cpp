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

    int N, M, i, j, a, b, k, **men, **women, *R, s, t;
    ifstream fin;
    fin.open ("N04/marriage.in");
    fin >> t;

    for (k=0; k<t; k++) {

        fin >> M >> N;
        
        men = new int* [M], women = new int* [N];
        for (i=0; i<M; i++) {
            men[i] = new int[3];
            for (j=0; j<3; j++) fin >> men[i][j];
        };
        for (i=0; i<N; i++) {
            women[i] = new int[3];
            for (j=0; j<3; j++) fin >> women[i][j];
        };

        Graph G (M+N+2);
        for (i=0; i<M; i++) {
            G.addEdge (0, i+1, 1);
            for (j=0; j<N; j++) {
                if (abs(men[i][0]-women[j][0]) <= 12 
                    && abs(men[i][1]-women[j][1]) <= 5
                    && men[i][2] == women[j][2]) G.addEdge(i+1, M+j+1, 1);
                else G.addEdge(i+1, M+j+1, 0);
            };
        };
        for (i=0; i<N; i++) G.addEdge(M+i+1, M+N+1, 1);

        cout << "Case " << k+1 << ": " << G.EdmondsKarp(0, M+N+1) << endl;
    };

    fin.close();
    return 0;
};