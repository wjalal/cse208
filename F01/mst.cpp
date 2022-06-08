#include <iostream>
#include <fstream>
#include <stack>
#include <list>
#include <vector>
#include <tuple>
#include <bits/stdc++.h>


using namespace std;

class Graph {
    int V, E, *dsuParent;
    vector<tuple<double, int, int>> edgeList;
    int dsuRoot (int v);
    void dsuConnect (int v, int w);

    public:
        bool* isVisited;
        void clearVisits();
        Graph(int V);
        void addEdge (int v, int w, double weight);
        void popEdge(); 
        bool hasCycle();
        void printEdges();
        double printKruskalMST();
        double printPrimMST (int v);
}; 


Graph::Graph (int V) {
    this->V = V, this->E = 0;
    this->isVisited = new bool[V];
    this->dsuParent = new int[V];
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

int Graph::dsuRoot (int v) {
    if (dsuParent[v] == v) return v;
    return dsuRoot (dsuParent[v]);
};

void Graph::dsuConnect (int v, int w) {
    dsuParent[v] = w;
};


bool Graph::hasCycle() {
    int i;
    for (i=0; i<V; i++) dsuParent[i] = i;
    for (i=0; i<E; i++) {
        int rV = dsuRoot(get<1>(edgeList[i])), rW = dsuRoot(get<2>(edgeList[i]));
        if (rV == rW) return true;
        dsuConnect (rV, rW);
    };
    return false;
};

double Graph::printKruskalMST() {
    sort (edgeList.begin(), edgeList.end());
    int i; Graph MST(V); double cost = 0.0;
    for (i=0; i<E; i++) {
        MST.addEdge (get<1>(edgeList[i]), get<2>(edgeList[i]), get<0>(edgeList[i]));
        if (MST.hasCycle()) {
            MST.popEdge();
        } else cost += get<0>(edgeList[i]);
    };
    MST.printEdges();
    return cost;
};

double Graph::printPrimMST (int v = 0) {
    int i, j, I, J, c=0; double cost = 0.0, currMin = INFINITY;
    vector<tuple<double, int>>* adjList = new vector<tuple<double, int>>[V]; Graph MST(V);
    for (i=0; i<E; i++) {
        int v = get<1>(edgeList[i]), w = get<2>(edgeList[i]);
        double weight = get<0>(edgeList[i]);
        adjList[v].push_back(make_tuple(weight, w));
        adjList[w].push_back(make_tuple(weight, v));
    };

    vector<int> visited;
    visited.push_back(v);

    while (visited.size() < V) {
        for (i=0; i < visited.size(); i++) {
            for (j=0; j < adjList[visited[i]].size(); j++) {
                if (get<0>(adjList[visited[i]][j]) <= currMin) { 
                    c = get<1>(adjList[visited[i]][j]);
                    currMin = get<0>(adjList[visited[i]][j]);
                    I = i, J = j;
                };
            };
        };
        MST.addEdge (visited[I], c, currMin);
        if (MST.hasCycle()) MST.popEdge();
        else visited.push_back(c), isVisited[c] = true, cost +=  currMin;

        adjList[visited[I]].erase(adjList[visited[I]].begin() + J);
        for (int p=0; p < adjList[c].size(); p++) 
            if (get<1>(adjList[c][p]) == visited[I]) adjList[c].erase(adjList[c].begin() + p);
        currMin = INFINITY;
    };
    MST.printEdges();
    return cost;
};


int main() {
    int N, M, i, v, w;
    double weight, cost;
    ifstream fin;

    fin.open ("mst.in", ios::in);
    fin >> N >> M;
    Graph g(N);

    for (i=0; i<M; i++) {
        fin >> v >> w >> weight;
        g.addEdge (v, w, weight);
    };

    cout << "List of edges selected by Prim's: ";
    cost = g.printPrimMST();
    cout << "Cost of the minimum spanning tree (Prim's): " << cost << endl << endl;

    cout << "List of edges selected by Kruskal's: ";
    cost = g.printKruskalMST();
    cout << "Cost of the minimum spanning tree (Kruskal's): " << cost << endl << endl;
    
    fin.close();
    return 0;
};