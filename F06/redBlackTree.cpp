
#include<iostream>
#include <cmath>
#include<algorithm>
using namespace std;

enum color {
    RED, BLACK
};

template <typename T> class Node {

public:

    T key;
    Node<T> *p, *l, *r;
    color col;

    Node<T>(color col = RED, Node<T>* nil = NULL) {
        this->p = this->l = this->r = nil;
        this->col = col;
    };
    Node<T> (T key, color col = RED, Node<T>* nil = NULL) {
        this->p = this->l = this->r = nil;
        this->key = key, this->col = col;
    };

};


template <typename T> class RedBlackTree {

    Node<T> *nil, *root;

    void leftRotate (Node<T>* x) {
        Node<T>* y = x->r;
        x->r = y->l;
        if (y->l != nil) y->l->p = x;
        y->p = x->p;
        if (x->p == nil) root = y;
        else if (x == x->p->l) x->p->l = y;
        else x->p->r = y;
        y->l = x;
        x->p = y;
    };

    void rightRotate (Node<T>* x) {
        Node<T>* y = x->l;
        x->l = y->r;
        if (y->r != nil) y->r->p = x;
        y->p = x->p;
        if (x->p == nil) root = y;
        else if (x == x->p->r) x->p->r = y;
        else x->p->l = y;
        y->r = x;
        x->p = y;
    };

    void insertFixup (Node<T>* z) {
        cout << "ifu" << endl;
        while (z->p->col == RED) {
            if (z->p == z->p->p->l) {
                Node<T>* y = z->p->p->r;
                if (y->col == RED) {
                    z->p->col = BLACK;
                    y->col = BLACK;
                    z->p->p->col = RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->r) {
                        z = z->p;
                        leftRotate(z);
                    };
                    z->p->col = BLACK;
                    z->p->p->col = RED;
                    rightRotate(z->p->p);
                };
            } else {
                Node<T>* y = z->p->p->l;
                if (y->col == RED) {
                    z->p->col = BLACK;
                    y->col = BLACK;
                    z->p->p->col = RED;
                    z = z->p->p;
                } else {
                    if (z == z->p->l) {
                        z = z->p;
                        rightRotate(z);
                    };
                    z->p->col = BLACK;
                    z->p->p->col = RED;
                    leftRotate(z->p->p);
                };
            };
        };
    };

    void inOrder (Node<T>* x) {
        if (x == nil) return;
        inOrder (x->l);
        cout << x->key << " ";
        inOrder (x->r);
    };

public:

    RedBlackTree() {
        this->root = this->nil = new Node<T> (BLACK);
    };

    void insert (T key) {
        Node<T> *y = nil, *x = root, *z = new Node<T>(key, RED, nil);
        cout << "i" << endl;
        while (x != nil) {
            y = x;
            if (z->key < x->key) x = x->l;
            else x = x->r;
        };
        z->p = y;
        if (y == nil) root = z;
        else if (z->key < y->key) y->l = z;
        else y->r = z;
        insertFixup(z);
    };

    void printInOrder() {
        cout << "Inorder: " << endl;
        if (root == nil)
        cout << "Tree is empty" << endl;
        else inOrder(root);
        cout << endl;
    };

};


int main() {
    RedBlackTree<int> tree;

    tree.insert(7);
    tree.insert(3);
    tree.insert(18);
    tree.insert(10);
    tree.insert(22);
    tree.insert(8);
    tree.insert(11);
    tree.insert(26);
    tree.insert(2);
    tree.insert(6);
    tree.insert(13);
    
    tree.printInOrder();
    return 0;
};