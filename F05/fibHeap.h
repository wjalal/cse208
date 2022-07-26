
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

    fibNode<T> *min;
    T inf;
    int heapSize;

    void addToRootList (fibNode<T>* x) {
        x->p=nullptr;
        if(!min){
            min=x;
            min->l=min , min->r=min;
            return;
        }
        x->r = min->r, min->r->l = x;
        min->r = x, x->l = min;
        if(x->key < min->key) min=x;
        return;
    };

    fibNode<T>* rmvFromRootList (fibNode<T>* x) {
        if(x->r==x){
            return nullptr;
        }
        fibNode<T>* y = x->r;
        y->l->l->r=y;
        y->l=y->l->l;
        return y;
    };

    fibNode<T>* link (fibNode<T>* y, fibNode<T>* x) {
        if(x->key > y->key) swap(x,y);
        x->p = nullptr, y->p = nullptr;
        y->r = y->l = y;
        x->r = x->l = x;
        if(x->deg > 0){
            y->r = x->ch->r, x->ch->r->l = y;
            x->ch->r = y; y->l = x->ch;
        };
        x->ch=y;
        y->p=x;
        x->deg++;
        y->mark = false;
        return x;
    };

    void consolidate() {
        int D = 31, i;
        fibNode<T>** A = new fibNode<T>*[D];
        for (i=0; i<D; i++) A[i] = nullptr;
        int looped = 0;
        fibNode<T>* x=min;
        while(looped<2 && x) {
            fibNode<T>* y = x->l;
            //cout<< "bonk\n";
            if(x==min){
                looped++;
                if(looped==2) break;
            }
            while(A[x->deg]) {
                int d = x->deg;
                x = link (A[d], x);
                A[d] = nullptr;
            };
            A[x->deg] = x;
            x=y;
        };
        min = nullptr;
        for (i=0; i<D; i++) {
            if (A[i]) {
                addToRootList(A[i]);
            };
        };

    };

public:

    FibHeap (T inf = INT32_MAX) {
        this->min = nullptr;
        this->heapSize = 0, this->inf = inf;
    };

    void insert (T key){
        heapSize++;
        if(!min){
            min = new fibNode<T>(key);
            min->r = min->l = min;
            return;
        }
        fibNode<T>* x = new fibNode<T>(key);
        addToRootList(x);
    };

    T getMin() {
        return min->key;
    };

    int getHeapsize() {
        return heapSize;
    };

    T extractMin() {
        if(heapSize==0){
            cout<< "Empty\n";
            T z;
            return z;
        }
        heapSize--;
        T r=min->key;
        fibNode<T>* z=min;

        if(min->r == min){
            if(min->ch==nullptr){
                min=nullptr;
                heapSize=0;
            }
            else min=min->ch;
        }
        else{
            fibNode<T>* x=min->ch;
            min=rmvFromRootList(min);
            if(x){
                fibNode<T>* y=x->l;

                fibNode<T>* w=min->l;

                w->r = x , x->l = w;
                min->l = y, y->r = min;
            }
        }
        consolidate();
        return r;
    };

    bool empty(){
        return (heapSize==0) ;
    }
    void print() {
        cout << "|||||||||||||||||||||||||" << endl;
        min->travel(true);
        cout << "|||||||||||||||||||||||||" << endl;
    };

    void travel(){
        min->travel(true);
    }

    int size(){
        return heapSize;
    }

};
