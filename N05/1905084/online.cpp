#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <cstring>
#include "chainingHashTable.h"
#include "openAddressingHashTable.h"

char** getRandomWords (int N) {
    int i, j;
    char** words = new char*[N];
    for (i=0; i<N; i++) {
        words[i] = new char[8];
        for (j=0; j<7; j++) words[i][j] = 97 + abs(rand()) % 25;
        words[i][7] = '\0';
    };
    return words;
};

int main() {
    int N, i, j, k, q;
    double p = 0.4, st=0;
    cin >> N;
    int p10 = p*0.1*N, p1 = N*p;
    srand(time(0));
    char **wordsToSearch, **words = getRandomWords(N);

    ChainingHashTable<int> T (p1);

    bool* wordsTaken = new bool[p1];
    for (j=0; j<p1; j++) wordsTaken[j] = false;

    for (i=0, j=1; i<p1; i++, j++) {
        T.insert (words[i], i+1);
        if (j==100) {
            j = 1;

            wordsToSearch = new char*[p10];
            for (q=0; q<p10; q++) {
                int k  = abs(rand())%(p1);
                while (wordsTaken[k]) k  = abs(rand())%(p1);
                wordsToSearch[q] = words[k];
                wordsTaken[k] = true;
            };

            auto t1 = chrono::steady_clock::now();
            for (q=0; q<p10; q++) T.search(wordsToSearch[q]);
            auto t2 = chrono::steady_clock::now();
            st = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() / p10;  

            cout << "Avg search time before rehash: " << st << endl;
            cout << "Max chain length before rehash: " << T.getMaxChain() << endl;

            T.rehash(1.2);
            cout << "hello" << endl;
        };
    };



};