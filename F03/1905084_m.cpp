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

// double** squareMatrixMultiply (double** A, double** B, int n) {
//     int i, j, k;
//     double** C = new double*[n];
//     for (i=0; i<n; i++) C[i] = new double[n];

//     for (i=0; i<n; i++) {
//         for (j=0; j<n; j++) {
//             C[i][j] = 0;
//             for (k=0; k<n; k++) C[i][j] += (A[i][k] * B[k][j]);
//         };
//     };
//     return C;
// };

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
    
    double** extendShortestPaths (double** L, double** W) {
        int i, j, k;
        double** L_ = new double*[V];
        for (i=0; i<V; i++) L_[i] = new double[V];

        for (i=0; i<V; i++) {
            for (j=0; j<V; j++) {
                L_[i][j] = INFINITY;
                for (k=0; k<V; k++) 
                    L_[i][j] = min (L_[i][j], L[i][k] + W[k][j]);
            };
        };
        return L_;
    };
    
    public:
        bool* isVisited;
        void clearVisits();
        Graph(int V);
        vector<tuple<double, int>>*  getAdjList();
        double** getAdjMatrix();
        void addEdge (int v, int w, double weight);
        void popEdge(); 
        void printEdges();
        double** getMatMultAPSP();
        double** getFloydWarshallAPSP();
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
            if (i == j) W[i][j] = 0;
            else W[i][j] = INFINITY;
        };
    };
    for (i=0; i<E; i++) {
        int v = get<1>(edgeList[i]), w = get<2>(edgeList[i]);
        double weight = get<0>(edgeList[i]);
        W[v][w] = weight;
    };
    return W;
};

double** Graph::getMatMultAPSP() {
    int i, j, m;
    double** W = getAdjMatrix();
    double*** L = new double**[2*V];
    L[1] = W, m = 1;
    while (m < V-1) {
        L[2*m] = extendShortestPaths (L[m], L[m]);
        m *= 2;
    };
    cout << endl;
    return L[m];
};

double** Graph::getFloydWarshallAPSP() {
    int i, j, k;
    double** W = getAdjMatrix();
    double** D = new double*[V];
    for (i=0; i<V; i++) D[i] = new double[V];

    for (i=0; i<V; i++) 
        for (j=0; j<V; j++) 
            D[i][j] = W[i][j];

    for (k=0; k<V; k++)
        for (i=0; i<V; i++)
            for (j=0; j<V; j++)
                D[i][j] = min(D[i][j], D[i][k] + D[k][j]);

    return D;
};

int main (int argc, char** argv) {

    if (argc != 2) cout << "Invalid CLI args, defaulting to Floyd-Warshall algorithm" << endl;
    int N, M, i, v, w, s, d;
    double weight, cost;
    ifstream fin;
    fin.open ("F03/apsp.in");
    fin >> N >> M;
    Graph g(N);
    for (i=0; i<M; i++) {
        fin >> v >> w >> weight;
        g.addEdge (v-1, w-1, weight);
    };
    fin >> s >> d;

    if(argc == 2) {
        if(!strcasecmp(argv[1], "m")) {
            printMatrix (g.getMatMultAPSP(), N, N);
            return 0;
        } else if(strcasecmp(argv[1], "f")) 
            cout << "Invalid CLI args, defaulting to Floyd-Warshall algorithm" << endl;
    };
    printMatrix (g.getFloydWarshallAPSP(), N, N);

    fin.close();
    return 0;
};