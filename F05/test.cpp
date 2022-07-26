#include<iostream>
#include "minHeap.h"
#include "fibHeap.h"

using namespace std;

int main() {
    FibHeap<int> h2(INT32_MAX);
    int x, i;
    while (true) {
        cin >> x;

        if (x == -69) {
            h2.extractMin();
        } else {
            h2.insert(x);
        };
        h2.print();
    };
    return 0;
};