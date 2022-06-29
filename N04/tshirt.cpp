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
    char *s1 = new char[5], *s2 = new char[5], *output = new char[5];
;
    ifstream fin;
    fin.open ("N04/tshirt.in");
    fin >> t;

    for (k=0; k<t; k++) {

        fin >> N >> M;

        Graph G(8+M);
        
        men = new int* [M], women = new int* [N];
        for (i=0; i<M; i++) {
            fin >> s1 >> s2;
            if (!strcasecmp(s1, "xs")) a = 1;
            else if (!strcasecmp(s1, "s")) a = 2;
            else if (!strcasecmp(s1, "m")) a = 3;
            else if (!strcasecmp(s1, "l")) a = 4;
            else if (!strcasecmp(s1, "xl")) a = 5;
            else if (!strcasecmp(s1, "xxl")) a = 6;

            if (!strcasecmp(s2, "xs")) b = 1;
            else if (!strcasecmp(s2, "s")) b = 2;
            else if (!strcasecmp(s2, "m")) b = 3;
            else if (!strcasecmp(s2, "l")) b = 4;
            else if (!strcasecmp(s2, "xl")) b = 5;
            else if (!strcasecmp(s2, "xxl")) b = 6;

            for (j=0; j<6; j++) 
            G.addEdge (a, 7+i, 1);
            G.addEdge (b, 7+i, 1);
            G.addEdge (7+i, M+7, 1);
        };

        for (i=0; i<6; i++) G.addEdge (0, i+1, N);

        if (G.EdmondsKarp(0, M+7) < M) strcpy(output, "NO");
        else strcpy(output, "YES");
        cout << "Case " << k+1 << ": " << output << endl;
    };

    fin.close();
    return 0;
};