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

class Graph {
    int V, E, *prev;
    double *dist;
    vector<tuple<double, int, int>> edgeList;
    
    public:
        bool* isVisited;
        void clearVisits();
        Graph(int V);
        vector<tuple<double, int>>*  getAdjList();
        void addEdge (int v, int w, double weight);
        void popEdge(); 
        void printEdges();
        void runDijkstra (int s);
        bool runBellmanFord (int s);
        void printSSSP (int s, int d);
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
        cout << "(" << get<1>(edgeList[i]) << ", " << get<2>(edgeList[i]) << ", " << get<0>(edgeList[i]) << "), ";
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

bool Graph::runBellmanFord (int s) {
    int i, j;
    for (i=0; i<V; i++) dist[i] = 0.0, prev[i] = -1;
    dist[s] = 1.0;
    vector<tuple<double, int>>* adjList = getAdjList();

    for (i=0; i<V-1; i++) {
        for (j=0; j<E; j++) {
            int a = get<1>(edgeList[j]), b = get<2>(edgeList[j]);
            double weight = get<0>(edgeList[j]);
            if (dist[b] < dist[a] * weight) {
                dist[b] = dist[a] * weight;
                prev[b] = a;
            };
        }
    };

    for (j=0; j<E; j++) {
        int a = get<1>(edgeList[j]), b = get<2>(edgeList[j]), weight = get<0>(edgeList[j]);
        if (dist[b] < dist[a] * weight) {
            cout << "The graph contains a negative cycle." << endl;
            return false;
        };
    };
    return true;
};

void Graph::printSSSP (int s, int d) {
    cout << "Shortest path cost: " << dist[d] << endl;
    stack<int> S; int a = d;
    while (a != -1) S.push(a), a = prev[a];
    while (!S.empty()) cout << S.top() << "->", S.pop();
    cout << "\b\b  " << endl;
    // for (int i=0; i<V; i++) cout << dist[i] << ", ";
    // cout << endl;
};

int main (int argc, char** argv) {

    int N, M, i, v, w, s, d;
    double weight, cost;
    ifstream fin;
    fin.open ("F02/sssp.in");
    fin >> N >> M;
    Graph g(N);
    for (i=0; i<M; i++) {
        fin >> v >> w >> weight;
        g.addEdge (v, w, weight);
    };
    fin >> s >> d;

    if (g.runBellmanFord(s)) g.printSSSP (s,d);

    fin.close();
    return 0;
};