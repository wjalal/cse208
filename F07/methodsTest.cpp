#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "chainingHashTable.h"
#include "openAddressingHashTable.h"


int main() {
    srand (time(0));
    int i, j;
    ChainingHashTable<int>* T = new ChainingHashTable<int> (100);
    //OpenAddressingHashTable<int>* T = new OpenAddressingHashTable<int> (100, LINEAR);
    for (i=0; i<100; i++) {
        char* s = new char[8];
        for (j=0; j<7; j++) s[j] = 97 + abs(rand()) % 25;
        T->insert (s, i);
    };
    T->print();
    for (i=0; true; i++) {
        char f[8] = "       "; 
        cout << "find/delete who? ";
        cin >> f;
        cout << T->search(f)->key << endl;
        T->deleteKey(f);
        T->print();
    };
    return 0;
};
