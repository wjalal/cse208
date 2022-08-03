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


void findTimes (int N, ofstream& fout, HashTable<int>* T, double bdst[4][6], double adst[4][6], double bdnp[4][6], double adnp[4][6]) {
    int i, j, cr = T->getColRes();
    char **words, **wordsToSearch, **wordsToDelete, **wordsToSearch2;
    double lf;

    fout << ",Before Deletion,After Deletion" << endl;
    if (cr) fout << "Load Factor,Average Search Time,Average Number of Probes,Average Search Time,Average Number of Probes" << endl;
    else fout << "Load Factor,Average Search Time,Average Search Time" << endl;

    for (i=0, lf = 0.4; i < 6; i++, lf+=0.1) {
        int p = N * lf, p10 = p * 0.1;
        adnp[cr][i] = 0.0, bdnp[cr][i] = 0.0;
        bool* wordsTaken = new bool[p];
        for (j=0; j<p; j++) wordsTaken[j] = false;

        srand (time(0));
        words = getRandomWords (N*lf);
        for (j=0; j<p; j++) T->insert (words[j], j+1);
        
        wordsToSearch = new char*[p10];
        for (j=0; j<p10; j++) {
            int k  = abs(rand())%p;
            while (wordsTaken[k]) k  = abs(rand())%p;
            wordsToSearch[j] = words[k];
            wordsTaken[k] = true;
        };

        
        for (j=0; j<p; j++) wordsTaken[j] = false;
        wordsToDelete = new char*[p10];
        for (j=0; j<p10; j++) {
            int k  = abs(rand())%p;
            while (wordsTaken[k]) k  = abs(rand())%p;
            wordsToDelete[j] = words[k];
            wordsTaken[k] = true;
        };


        for (j=0; j<p; j++) wordsTaken[j] = false;
        wordsToSearch2 = new char*[p10];
        for (j=0; j<p10; j++) {
            if (j%2) {
                int k  = abs(rand())%p;
                while (wordsTaken[k]) k  = abs(rand())%p;
                wordsToSearch2[j] = words[k];
                wordsTaken[k] = true;
            } else wordsToSearch2[j] = wordsToDelete[j];
        };

        chrono::_V2::steady_clock::time_point t1, t2; 
        t1 = chrono::steady_clock::now();
        for (j=0; j<p10; j++) bdnp[cr][i] += T->search(wordsToSearch[j]);
        t2 = chrono::steady_clock::now();
        bdst[cr][i] = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() / p10;
        bdnp[cr][i] /= 1.0 * p10;

        for (j=0; j<p10; j++) T->deleteKey(wordsToDelete[j]);

        t1 = chrono::steady_clock::now();
        for (j=0; j<p10; j++) adnp[cr][i] +=  T->search(wordsToSearch2[j]);
        t2 = chrono::steady_clock::now();
        adst[cr][i] = chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() / p10;
        adnp[cr][i] /= 1.0 * p10;
        //cout << "hello " << i << endl;
        
        if (cr) fout << lf << "," << bdst[cr][i] << "," << bdnp[cr][i] << "," << adst[cr][i] << "," << adnp[cr][i] << endl;
        else fout << lf << "," << bdst[cr][i] << "," << adst[cr][i] << endl;
    };
    fout << endl << endl;
};

int main() {
    int N;
    ofstream fout;
    cin >> N;
    fout.open("F07/report_1905084.csv");
    HashTable<int>* T;  
    double bdst[4][6], adst[4][6], bdnp[4][6], adnp[4][6];
    fout << ",,N  =  " << N << endl << endl << endl;

    fout << "Table 1: Performance of separate chaining in various load factors" << endl << endl;
    T = new ChainingHashTable<int> (N);
    findTimes (N, fout, T, bdst, adst, bdnp, adnp);

    fout << "Table 2: Performance of linear probing in various load factors" << endl << endl;
    T = new OpenAddressingHashTable<int> (N, LINEAR);
    findTimes (N, fout, T, bdst, adst, bdnp, adnp);

    fout << "Table 3: Performance of quadratic probing in various load factors" << endl << endl;
    T = new OpenAddressingHashTable<int> (N, QUADRATIC);
    findTimes (N, fout, T, bdst, adst, bdnp, adnp);

    fout << "Table 4: Performance of double hashing in various load factors" << endl << endl;
    T = new OpenAddressingHashTable<int> (N, DOUBLE_HASH);
    findTimes (N, fout, T, bdst, adst, bdnp, adnp);

    for (int i=0; i<6; i++) {
        fout << "Table " << 5+i << ": Performance of various collision resolution methods in load factor " << 0.4 + 0.1*i << endl << endl;
        fout << ",Before Deletion,After Deletion" << endl;
        fout << ",Average Search Time,Average Number of Probes,Average Search Time,Average Number of Probes" << endl;
        for (int j=0; j<4; j++) {
            if (j == CHAINING) fout << "Separate Chaining,";
            else if (j == LINEAR) fout << "Linear Probing,";
            else if (j == QUADRATIC) fout << "Quadratic Probing,";
            else if (j == DOUBLE_HASH) fout << "Double Hashing,";
            if (j) fout << bdst[j][i] << "," << bdnp[j][i] << "," << adst[j][i] << "," << adnp[j][i] << endl;
            else fout << bdst[j][i] << "," << "N/A," << adst[j][i] << ",N/A" << endl;
        };
        fout << endl << endl;
    };


    fout.close();
    return 0;

};