#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

enum colResType {
    CHAINING,
    LINEAR,
    QUADRATIC,
    DOUBLE_HASH
};

template <typename T> class Pair {

public:
    char* key;
    T value;
    Pair<T> (char* key, T value) {
        this->key = key;
        this->value = value;
    };
};

template <typename T> class PairNode {

public:
    char* key;
    T value;
    PairNode<T> *prev, *next;

    PairNode<T> (char* key, T value) {
        this->key = key, this->value = value;
        this->prev = this->next = this;
    };

    void append (PairNode<T>* x) {
        x->next = this->next, this->next->prev = x;
        this->next = x, x->prev = this;
    };
};

template <typename E> class HashTable {

public:
    virtual int insert (char* key, E value) = 0;
    virtual int search (char* key) = 0;
    virtual void deleteKey (char* key) = 0;
    virtual void print() = 0;
    virtual colResType getColRes() = 0;
};
