
#include<iostream>
#include <cmath>
#include<algorithm>
using namespace std;

template <typename T> class fibNode {

public:

    T key;
    fibNode<T> *p, *l, *r, *ch;
    bool mark;
    int deg;

    fibNode<T>() {
        this->p = nullptr, this->ch = nullptr, this->l = this->r = this;
        this->deg = 0, this->mark = false;
    };
    fibNode<T> (T key) {
        this->p = nullptr, this->ch = nullptr, this->l = this->r = this;
        this->deg = 0, this->mark = false, this->key = key;
    };

    void travel(bool loop) {
        if (!this){
            cout<< "Heap is empty, can't travel.\n";
            return;
        }
        if (!p) cout<< "hello--------- \n";
        cout<< "entered "<< key << "\n";
        if (ch) {
            cout<< "child\n";
            ch->travel(true);
        };
        fibNode<T>* hello = this;
        while (hello->r != this && loop){
            hello = hello->r;
            hello->travel(false);
        }
        cout<< "bye "<< key << "\n";
    };
};


template <typename T> class FibHeap{

    fibNode<T> *min, *root;
    T inf;
    int heapSize;

    void addToRootList (fibNode<T>* x) {
        x->r = root->r, root->r->l = x;
        root->r = x, x->l = root;
    };

    void rmvFromRootList (fibNode<T>* x) {
        x->l->r = x->r;
        x->r->l = x->l;
    };

    void link (fibNode<T>* y, fibNode<T>* x) {
        rmvFromRootList(y);
        y->p = x, y->r = y->l = y;
        if (!x->ch) x->ch = y;
        else {
            y->r = x->ch->r, x->ch->r->l = y;
            x->ch->r = y; y->l = x->ch;
        };
        x->deg++;
        y->mark = false;
    };

    void consolidate() {
        int D = 31, i;
        fibNode<T>** A = new fibNode<T>*[D];
        for (i=0; i<D; i++) A[i] = nullptr;
        bool looped = false;
        for (fibNode<T>* x=root; !looped; x=x->r) {
            int d = x->deg;
            while (A[d] && d<D) {
                fibNode<T>* y = A[d];
                if (x->key > y->key) swap(x, y);
                link (y, x);
                A[d] = nullptr;
                d++;
            };
            A[d] = x;
            if(x == root->l) looped = true;
            cout << "hc" << endl;
        };
        min = nullptr;
        for (i=0; i<D; i++) {
            if (A[i]) {
                if (!min) root = min = A[i];
                else {
                    addToRootList (A[i]);
                    if (A[i]->key < min->key) min = A[i];
                };
            };
        };

        // fibNode<T>* x = min;
        // i = 0;
        // while (i<2) {
        //     fibNode<T>* y = x->r;
        //     if (x == min) {
        //         i++;
        //         if (i==2) break;
        //     };
        //     while (A[x->deg]) {
        //         int k = x->deg;
        //         link(A[k], x);
        //         A[k] = nullptr;
        //     };
        //     A[x->deg] = x;
        //     x = y;
        // };
        
    };

public:

    FibHeap (T inf = INT32_MAX){
        this->min = this->root = nullptr;
        this->heapSize = 0, this->inf = inf;
    };

    void insert (T key){
        fibNode<T>* x = new fibNode<T>(key);
        if (!min) root = min = x;
        else {
            addToRootList(x);
            if (x->key < min->key) min = x;
        };
        heapSize++;
        //cout << get<1>(key) << " inserted" << endl;
    };

    T getMin() {
        return min->key;
    };

    int getHeapsize() {
        return heapSize;
    };

    T extractMin() {
        fibNode<T>* z = min;
        T r;
        if (z) {
            r = z->key;
            fibNode<T>* x = z->ch;
            if (x) {
                addToRootList(x), x->p = nullptr;
                for (x=z->ch->r; x!=z->ch; x=x->r) {
                    addToRootList(x);
                    x->p = nullptr;
                };
            };        
            rmvFromRootList(z);
            if (z == z->r) min = nullptr;
            else {
                min = z->r;
                consolidate();
                cout << "?" << endl;
            };
            heapSize--;
            cout << "hem" << endl;
            delete z;
        };
        //cout << get<1>(r) << " " << heapSize << endl;
        return r;
    };

    void print() {
        cout << "|||||||||||||||||||||||||" << endl; 
        min->travel(true);
        cout << "|||||||||||||||||||||||||" << endl;
    };

};
