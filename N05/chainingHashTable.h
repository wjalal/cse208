#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "hashFunctions.h"
#include "hashTable.h"

bool check_prime(int n) {
  bool is_prime = true;

  // 0 and 1 are not prime numbers
  if (n == 0 || n == 1) {
    is_prime = false;
  }

  for (int i = 2; i <= n / 2; ++i) {
    if (n % i == 0) {
      is_prime = false;
      break;
    }
  }

  return is_prime;
}

template <typename E> class ChainingHashTable : public HashTable<E> {

    int N;
    PairNode<E>** T;

    PairNode<E>* find (char* key) {
        int i = h1(key, N) % N;
        if (!T[i]) return nullptr;
        PairNode<E>* r;
        if (!strcmp(T[i]->key, key)) return T[i];
        for (r = T[i]->next; r != T[i]; r = r->next) 
            if (!strcmp(r->key, key)) return r;
        return nullptr;
    };

public:

    ChainingHashTable<E> (int N) {
        this->N = N;
        this->T = new PairNode<E>* [N];
        for (int i=0; i<N; i++) T[i] = nullptr;
    };

    int insert (PairNode<E>* x) {
        int i = h1(x->key, N) % N;
        if (!T[i]) T[i] = x;
        else T[i]->append(x);
        return i;
    };

    int insert (char* key, E value) {
        PairNode<E>* x = new PairNode<E> (key, value);
        return insert(x);
    };

    int search (char* key) {
        if (find(key)) return 0;
        return -1;
    };

    void deleteKey (char* key) {
        PairNode<E>* f = find(key);
        if (!f) return;
        int i = h1(key, N) % N;
        if (f->next == f) T[i] = nullptr;
        else {
            if (T[i] == f) T[i] = f->next;
            f->prev->next = f->next;
            f->next->prev = f->prev;
        };
        //delete f;
    };

    void print() {
        for (int i=0; i<N; i++) {
            cout << i+1 << "  ";
            if (!T[i]) cout << "---" << endl;
            else {
                cout << T[i]->key << "  ";
                for (PairNode<E>* r = T[i]->next; r != T[i]; r = r->next)
                    cout << r->key << "  ";
                cout << endl;
            };
        };
    };

    colResType getColRes() {
        return CHAINING;
    };

    int getMaxChain() {
        int maxchain = 0, i;
        for (i=0; i<N; i++) {
            int chain = 1;
            if (!T[i]) continue;
            for (PairNode<E>* r = T[i]->next; r != T[i]; r = r->next) {
                chain++;
            };
            if (chain > maxchain) maxchain = chain;
        };
        return maxchain;
    }

    void rehash (double sizeFactor) {
        int i, j, N0 = N;
        char** copy = new char*[N];
        for (i=0, j=0; i<N; i++) {
            if (!T[i]) continue;
            PairNode<E>* r;
            copy[i] = T[i]->key;
            j = 1;
            for (r = T[i]->next; r != T[i]; r = r->next, j++) {
                copy[i+j] = r->key;
            };
            i += j;
            
        };

        for (i=0; i<N; i++) {
            cout << "hello" << i << endl;
            cout << copy[i] << endl;
            deleteKey(copy[i]);
        };

        N *= sizeFactor;
        while (!check_prime(N)) N++;
        for (i=0; i<N0; i++) insert(copy[i], i+1);

    };
};