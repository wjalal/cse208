#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "chainingHashTable.h"
#include "openAddressingHashTable.h"


int main() {
    srand (time(0));
    const int SIZE = 97;
    int i, j;
    //ChainingHashTable<int>* T = new ChainingHashTable<int> (100);
    OpenAddressingHashTable<int>* T = new OpenAddressingHashTable<int> (SIZE, QUADRATIC);
    for (i=0; i<97; i++) {
        char* s = new char[8];
        for (j=0; j<7; j++) s[j] = 97 + abs(rand()) % 25;
        s[7] = '\0';
        T->insert (s, i);
    };
    T->print();
    for (i=0; i<3; i++) { 
        cout << "find/delete who? " << endl;
        char* f = new char[8];
        cin >> f;
        cout << T->search(f) << endl;
        T->deleteKey(f);
        T->print();
    };
    for (i=0; i<3; i++) {
        cout << "insert: ";
        char* f = new char[8];
        cin >> f;
        T->insert(f, 420+i);
        T->print();
    };
    return 0;
};
