#include <iostream>
#include <fstream>
#include <stack>
#include <list>
#include <vector>
#include <tuple>
#include <bits/stdc++.h>
#include <queue>
#include <cstring>
#include <ctime>
#include "minHeap.h"
#include "fibHeap.h"

using namespace std;

class Graph {
    int V, E, *prev;
    double *dist;
    vector<tuple<double, int, int>> edgeList;
    
    public:
        bool* isVisited;
        void clearVisits();
        Graph(int V);
        vector<tuple<double, int>>*  getAdjList();
        Graph getTranspose();
        void addEdge (int v, int w, double weight);
        void popEdge(); 
        void printEdges();
        void runDijkstraBin (int s);
        void runDijkstraFib (int s);
        void printSSSP (int s, int d, ofstream& fout);
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

Graph Graph::getTranspose() {
    Graph gT(V);
    for (int e=0; e<E; e++)
        gT.addEdge (get<2>(edgeList[e]), get<1>(edgeList[e]), get<0>(edgeList[e]));
    return gT;
};

void Graph::runDijkstraFib (int s) {
    int i, v;
    for (i=0; i<V; i++) dist[i] = INFINITY, prev[i] = -1;
    dist[s] = 0;
    vector<tuple<double, int>>* adjList = getAdjList();

    FibHeap<tuple<double, int>> H(make_tuple(INFINITY, INT32_MAX));
    H.insert(make_tuple(dist[s], s));
    
    while (H.getHeapsize() > 0) {
        int u = get<1> (H.extractMin());
        for (v=0; v < adjList[u].size(); v++) {
            int a = u, b = get<1>(adjList[u][v]); 
            double weight = get<0>(adjList[u][v]);
            if (dist[b] > dist[a] + weight) {
                //cout << "." << endl;
                dist[b] = dist[a] + weight;
                prev[b] = a;
                H.insert(make_tuple(dist[b], b));
            };
        };
    };
};

void Graph::runDijkstraBin (int s) {
    int i, v;
    for (i=0; i<V; i++) dist[i] = INFINITY, prev[i] = -1;
    dist[s] = 0;
    vector<tuple<double, int>>* adjList = getAdjList();

    MinHeap<tuple<double, int>> H(make_tuple(INFINITY, INT32_MAX));
    H.insert(make_tuple(dist[s], s));
    
    while (H.getHeapsize() > 0) {
        int u = get<1> (H.extractMin());
        for (v=0; v < adjList[u].size(); v++) {
            int a = u, b = get<1>(adjList[u][v]); 
            double weight = get<0>(adjList[u][v]);
            if (dist[b] > dist[a] + weight) {
                //cout << "." << endl;
                dist[b] = dist[a] + weight;
                prev[b] = a;
                H.insert(make_tuple(dist[b], b));
            };
        };
    };
};

void Graph::printSSSP (int s, int d, ofstream& fout) {
    int l = 0,  a = d;
    while (a != -1) l++, a = prev[a];
    fout << l-1 << " " << dist[d] << " ";
};

int main () {

    int N, M, i, v, w, s, d, k;
    clock_t t1, t2;
    double weight, cost;
    ifstream fin;
    ofstream fout;
    fin.open ("F05/graph.in");
    fout.open ("F05/output.out");
    fin >> N >> M;
    Graph g(N);
    for (i=0; i<M; i++) {
        fin >> v >> w >> weight;
        g.addEdge (v, w, weight);
        g.addEdge (w, v, weight);
    };
    fin.close();

    fin.open ("F05/pairs.in");
    fin >> k;
    for (i=0; i<k; i++) {
        fin >> s >> d;
        t1 = clock();
        g.runDijkstraBin(s);
        t2 = clock();
        g.printSSSP (s, d, fout);
        fout << double(t2 - t1) / CLOCKS_PER_SEC * 1e6 << " ";
        t1 = clock();
        g.runDijkstraFib(s);
        t2 = clock();
        fout << double(t2 - t1) / CLOCKS_PER_SEC * 1e6 << endl;
    };

    fin.close();
    fout.close();
    return 0;
};