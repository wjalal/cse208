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
        Graph getTranspose();
        void addEdge (int v, int w, double weight);
        void popEdge(); 
        void printEdges();
        void runDijkstra (int s, bool ssdp);
        bool runBellmanFord (int s, bool ssdp);
        void printSSSP (int s, int d);
        void printSDSP (int s, int d);
        int getSSSPDest (int s);
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

void Graph::runDijkstra (int s, bool sdsp = false) {
    int i, v;
    for (i=0; i<V; i++) dist[i] = INFINITY, prev[i] = -1;
    dist[s] = 0;
    vector<tuple<double, int>>* adjList = sdsp? getTranspose().getAdjList() : getAdjList();

    priority_queue <tuple<double, int>, vector<tuple<double, int>>, greater<tuple<double,int>> > Q;
    Q.push(make_tuple(dist[s], s));

    while (!Q.empty()) {
        int u = get<1> (Q.top());
        Q.pop();
        for (v=0; v < adjList[u].size(); v++) {
            int a = u, b = get<1>(adjList[u][v]), weight = get<0>(adjList[u][v]);
            if (dist[b] > dist[a] + weight) {
                dist[b] = dist[a] + weight;
                prev[b] = a;
                Q.push(make_tuple(dist[b], b));
            };
        };
    };
};

bool Graph::runBellmanFord (int s, bool sdsp = false) {
    int i, j;
    for (i=0; i<V; i++) dist[i] = INFINITY, prev[i] = -1;
    dist[s] = 0;
    vector<tuple<double, int>>* adjList = sdsp? getTranspose().getAdjList() : getAdjList();

    for (i=0; i<V-1; i++) {
        for (j=0; j<E; j++) {
            int a = get<1>(edgeList[j]), b = get<2>(edgeList[j]), weight = get<0>(edgeList[j]);
            if (dist[b] > dist[a] + weight) {
                dist[b] = dist[a] + weight;
                prev[b] = a;
            };
        }
    };

    for (j=0; j<E; j++) {
        int a = get<1>(edgeList[j]), b = get<2>(edgeList[j]), weight = get<0>(edgeList[j]);
        if (dist[b] > dist[a] + weight) {
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
};

void Graph::printSDSP (int s, int d) {
    cout << "Shortest path cost: " << dist[s] << endl;
    stack<int> S; int a = s;
    while (a != -1) S.push(a), a = prev[a];
    while (!S.empty()) cout << S.top() << "->", S.pop();
    cout << "\b\b  " << endl;
};

int Graph::getSSSPDest (int s) {
    double minDist = INFINITY; int i, d;
    // for (i=0; i<V; i++) cout << dist[i] << ", ";
    // cout << endl;
    for (i=0; i<V; i++) 
        if (dist[i] < minDist && i != s) 
            minDist = dist[i], d = i;
    return d;
};


int main (int argc, char** argv) {

    if (argc != 2) cout << "Invalid CLI args, defaulting to Dijkstra's algorithm" << endl;
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

    if(argc == 2) {
        if(!strcasecmp(argv[1], "b")) {
            if (g.runBellmanFord(s)) g.printSSSP (s,d);
            return 0;
        } else if(strcasecmp(argv[1], "d")) 
            cout << "Invalid CLI args, defaulting to Dijkstra's algorithm" << endl;
    };
    g.runDijkstra(s), g.printSDSP (s, d);
    //cout << g.getSSSPDest(s) << endl;
    //g.printSSSP (s, g.getSSSPDest(s));

    fin.close();
    return 0;
};