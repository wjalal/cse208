#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>


enum colResType {
    LINEAR,
    QUADRATIC,
    DOUBLE_HASH
};

template <typename E> class OpenAddressingHashTable {

    int N;
    Pair<E>** T;
    colResType colRes;

    unsigned int h (char* k, int i) {
        if (colRes == LINEAR) return (h1(k, N) + i) % N;
        else if (colRes == QUADRATIC) return (h1(k, N) + 131*i + 67*i*i) % N;
        else if (colRes == DOUBLE_HASH) return (h1(k, N) + i * h2(k, N)) % N;
        return h2(k, N);
    };

public:

    OpenAddressingHashTable<E> (int N, colResType colRes) {
        this->N = N, this->colRes = colRes;
        this->T = new Pair<E>* [N];
        for (int i=0; i<N; i++) T[i] = nullptr;
    };

    int insert (Pair<E>* x) {
        int i = 0, j;
        while (i < N) {
            j = h (x->key, i);
            if (!T[j]) {
                T[j] = x;
                break;
            } else i++;
        };
        return j;
    };

    int insert (char* key, E value) {
        Pair<E>* x = new Pair<E> (key, value);
        return insert(x);
    };

    Pair<E>* search (char* key) {
        
    };

    void print() {
        for (int i=0; i<N; i++) cout << i+1 << " " << T[i]->key << "  "  << endl;
    };

};