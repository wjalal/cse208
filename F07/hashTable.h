#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

template <typename T> class Pair {

public:
    char* key;
    T value;
    Pair<T> (char* key, T value) {
        this->key = key;
        this->value = value;
    };
};

template <typename T> class PairNode : public Pair<T> {

public:
    PairNode<T> *prev, *next;

    PairNode<T> (char* key, T value) : Pair<T> (key, value) {
        this->prev = this->next = this;
    };

    void append (PairNode<T>* x) {
        x->next = this->next, this->next->prev = x;
        this->next = x, x->prev = this;
    };
};

template <typename E> class HashTable {

public:
    virtual int insert (char* key, E value);
    virtual Pair<E>* find (char* key);
    virtual void print();
};
