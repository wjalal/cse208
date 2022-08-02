#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>


template <typename E> class OpenAddressingHashTable : public HashTable<E> {

    int N;
    Pair<E>** T;
    colResType colRes;
    Pair<E>* DELETED;

    unsigned int h (char* k, int i) {
        if (colRes == LINEAR) return (h1(k, N) + i) % N;
        else if (colRes == QUADRATIC) return (h1(k, N) + 7*i + 37*i*i) % N;
        else if (colRes == DOUBLE_HASH) return (h1(k, N) + i * h2(k, N)) % N;
        return h2(k, N);
    };

public:

    OpenAddressingHashTable<E> (int N, colResType colRes) {
        this->DELETED = new Pair<E> (nullptr, 0);
        this->N = N, this->colRes = colRes;
        this->T = new Pair<E>* [N];
        for (int i=0; i<N; i++) T[i] = nullptr;
    };

    int insert (Pair<E>* x) {
        int i = 0, j;
        while (i < N) {
            j = h (x->key, i);
            if (!T[j] || T[j] == DELETED) {
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

    int search (char* key) {
        int i = 0, j = h(key, i), pCount = 0;
        while (T[j] && i<N) {
            j = h(key, i);
            if (!T[j] || T[j]==DELETED) return pCount;
            //cout << "hello" << endl;
            if (!strcmp(T[j]->key, key)) return pCount;
            i++, pCount++;
        };
        return pCount;
    };

    void deleteKey (char* key) {
        int i = 0, j = h(key, i);
        while (T[j] && i<N) {
            j = h(key, i);
            if (!T[j] || T[j]==DELETED) return;
            if (!strcmp(T[j]->key, key)) {
                T[j] = DELETED;
                break;
            };
            i++;
        };
    };

    void print() {
        for (int i=0; i<N; i++) {
            if (!T[i] || T[i] == DELETED) cout << "---" << endl;
            else cout << i+1 << " " << T[i]->key << "  " <<  (int) (T[i]->key)[7] << endl;
        };
    };

    colResType getColRes() {
        return this->colRes;
    };

};