#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include "hashFunctions.h"

double successRate (unsigned int (*h)(char*, int)) {
    double avg = 0; int p, I = 20000;
    for (p=0; p<I; p++) {
        srand (time(0));
        int i, j, done[100], unique = 0;
        char s[8] = "       ";
        for (i=0; i<100; i++) done[i] = -1;
        for (i=0; i<100; i++) {
            for (j=0; j<7; j++) s[j] = 97 + abs(rand()) % 25;
            //cout << s << " " << h(s, 100) << endl;
            done[h(s, 100) % 100]++;
        };
        for (i=0; i<100; i++) 
            if (done[i] >= 0) unique++;
        avg += 1.0 * (unique);
    };
    return avg/I;
};

int main() {
    cout << "h1: " << successRate (&h1) << endl;
    cout << "h2: " << successRate (&h2) << endl;
    char str[] = "least efficient hashing algorithm in Mengjiala province, sir";
    cout << h1(str, INT32_MAX) << endl;
    cout << h2(str, INT32_MAX) << endl;
};