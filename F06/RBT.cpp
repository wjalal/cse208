#include <iostream>
#include <fstream>

using namespace std;


enum color {
    RED, BLACK
};

template <typename T> class Node {
    public:
    T key;
    Node<T> *p, *l, *r;
    color col;
};

template <typename T> class RedBlackTree {

    Node<T> *root, *nil;

    void leftRotate(Node<T>* x) {
        Node<T>* y = x->r;
        x->r = y->l;
        if (y->l != nil) y->l->p = x;
        y->p = x->p;
        if (x->p == nullptr) this->root = y;
        else if (x == x->p->l) x->p->l = y;
        else x->p->r = y;
        y->l = x, x->p = y;
    };

    void rightRotate(Node<T>* x) {
        Node<T>* y = x->l;
        x->l = y->r;
        if (y->r != nil) y->r->p = x;
        y->p = x->p;
        if (x->p == nullptr) this->root = y;
        else if (x == x->p->r) x->p->r = y;
        else x->p->l = y;
        y->r = x, x->p = y;
    };

    void insertFixUp(Node<T>* z) {
        Node<T>* y = new Node<T>();
        while (z->p->col == RED) {
            if (z->p == z->p->p->r) {
                y = z->p->p->l;
                if (y->col == RED) {
                    y->col = BLACK;
                    z->p->col = BLACK;
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
            } else {
                y = z->p->p->r;
                if (y->col == RED) {
                    y->col = BLACK;
                    z->p->col = BLACK;
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
            };
            if (z == root) break;
        };
        root->col = BLACK;
    };

    void transplant(Node<T>* u, Node<T>* v) {
        if (!u->p) root = v;
        else if (u == u->p->l) u->p->l = v;
        else u->p->r = v;
        v->p = u->p;
    };

    void deleteFixUp (Node<T>* x) {
        Node<T>* w = new Node<T>();
        while (x != root && x->col == BLACK) {
            if (x == x->p->l) {
                w = x->p->r;
                if (w->col == RED) {
                    w->col = BLACK;
                    x->p->col = RED;
                    leftRotate(x->p);
                    w = x->p->r;
                };

                if (w->l->col == BLACK && w->r->col == BLACK) {
                    w->col = RED;
                    x = x->p;
                } else {
                    if (w->r->col == BLACK) {
                        w->l->col = BLACK;
                        w->col = RED;
                        rightRotate(w);
                        w = x->p->r;
                    };
                    w->col = x->p->col;
                    x->p->col = BLACK;
                    w->r->col = BLACK;
                    leftRotate(x->p);
                    x = root;
                };
            } else {
                w = x->p->l;
                if (w->col == RED) {
                    w->col = BLACK;
                    x->p->col = RED;
                    rightRotate(x->p);
                    w = x->p->l;
                };
                if (w->r->col == BLACK && w->l->col == BLACK) {
                    w->col = RED;
                    x = x->p;
                } else {
                    if (w->l->col == BLACK) {
                        w->r->col = BLACK;
                        w->col = RED;
                        leftRotate(w);
                        w = x->p->l;
                    };

                    w->col = x->p->col;
                    x->p->col = BLACK;
                    w->l->col = BLACK;
                    rightRotate(x->p);
                    x = root;
                };
            };
        };
        x->col = BLACK;
    };

    Node<T>* searchHelp (Node<T>* x, T key) {
        if (!x || key == x->key) return x;
        if (key < x->key) return searchHelp(x->l, key);
        return searchHelp(x->r, key);
    };

    int countHelp (Node<T>* x, T key) {
        if (!x) return 0;
        int count = 0;
        if (key > x->key) {
            count += (countHelp (x->r, key) + 1);
        } else count += countHelp(x->l, key);
        return count;
    };

    // void printHelp(Node<T>* root, string indent, bool last) {
    //     if (root != nil) {
    //         cout << indent;
    //         if (last) {
    //             cout << "R----";
    //             indent += "   ";
    //         } else {
    //             cout << "L----";
    //             indent += "|  ";
    //         }

    //         string sColor = root->col == RED ? "RED" : "BLACK";
    //         cout << root->key << "(" << sColor << ")" << endl;
    //         printHelp(root->l, indent, false);
    //         printHelp(root->r, indent, true);
    //     };
    // };


    // void inOrder (Node<T>* x) {
    //     if (x == nil) return;
    //     inOrder (x->l);
    //     cout << x->key << " ";
    //     inOrder (x->r);
    // };


public:

    RedBlackTree() {
        nil = new Node<T>();
        nil->col = BLACK, nil->l = nil->r = nullptr;
        root = nil;
    };

    void insert (T key) {
        Node<T> *z = new Node<T>(), *y = nullptr, *x = this->root;
        z->p = nullptr, z->l = nil, z->r = nil;
        z->key = key, z->col = RED;
        while (x != nil) {
            y = x;
            if (z->key < x->key) x = x->l;
            else x = x->r;
        };
        z->p = y;
        if (!y) root = z;
        else if (z->key < y->key) y->l = z;
        else y->r = z;
        if (!z->p) {
            z->col = BLACK;
            return;
        };
        if (!z->p->p) return;
        insertFixUp(z);
    };

    bool deleteNode (T key) {
        Node<T> *z = nil, *x, *y, *u = root;

        while (u != nil) {
            if (u->key == key) z = u;
            if (u->key <= key) u = u->r;
            else u = u->l;
        };
        if (z == nil) return false;

        y = z;
        color yColInit = y->col;
        if (z->l == nil) {
            x = z->r;
            transplant(z, z->r);
        } else if (z->r == nil) {
            x = z->l;
            transplant(z, z->l);
        } else {
            while (z->r->l != nil) z->r = z->r->l;
            y = z->r;
            yColInit = y->col;
            x = y->r;
            if (y->p == z) x->p = y;
            else {
                transplant(y, y->r);
                y->r = z->r;
                y->r->p = y;
            };
            transplant(z, y);
            y->l = z->l;
            y->l->p = y;
            y->col = z->col;
        };
        delete z;
        if (yColInit == BLACK) deleteFixUp(x);
        //cout << "del" << endl;
        return true;
    };

    bool search (T key) {     
        Node<T>* r = searchHelp (root, key);
        if (!r) return false;
        return true;
    };

    Node<T>* getRoot() {
        return root;
    };

    int getCount (T key) {
        return countHelp (root, key);
    };

    // void printTree() {
    //     if (root) printHelp(this->root, "", true);
    // };

    // void printInOrder() {
    //     inOrder(root);
    //     cout << endl;
    // };
};

int main() {
    int N, e, x, i;
    RedBlackTree<int> T;
    ifstream fin;
    fin.open ("F06/rbt.in");
    
    fin >> N;
    cout << N << endl;
    for (i=0; i<N; i++) {
        fin >> e >> x;
        cout << e << " " << x << " ";
        if (e == 0) {
            if (T.deleteNode(x)) cout << 1 << endl;
            else cout << 0 << endl; 
        } else if (e == 1) {
            if (T.search(x)) cout << 0 << endl;
            else {
                T.insert(x);
                cout << 1 << endl;
            };
        } else if (e == 2) {
            if (T.search(x)) cout << 1 << endl;
            else cout << 0 << endl;
        } else if (e == 3) {
            cout << T.getCount(x) << endl;
        };
    };
    fin.close();
    return 0;
};

// int main() {
//     RedBlackTree<int> bst;
//     bst.insert(55);
//     bst.insert(40);
//     bst.insert(65);
//     bst.insert(60);
//     bst.insert(75);
//     bst.insert(57);

//     bst.printTree();
//     cout << endl
//          << "After deleting" << endl;
//     bst.deleteNode(40);
//     bst.deleteNode(75);
//     bst.deleteNode(57);
//     bst.printTree();
//     bst.printInOrder();
// }

// int main() {
//     RedBlackTree<int> tree;

//     tree.insert(7);
//     tree.insert(3);
//     tree.insert(18);
//     tree.insert(10);
//     tree.insert(22);
//     tree.insert(8);
//     tree.insert(11);
//     tree.insert(26);
//     tree.insert(2);
//     tree.insert(6);
//     tree.insert(13);
    
//     tree.printInOrder();
    
//     cout << endl << "Deleting" << endl;
 
//     tree.deleteNode(18);
//     tree.deleteNode(11);
//     tree.deleteNode(3);
//     tree.deleteNode(10);
//     tree.deleteNode(22);
    
//     tree.printInOrder();
//     return 0;
// };