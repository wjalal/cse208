
#include<iostream>
#include <cmath>
#include<algorithm>
using namespace std;

template <typename T> class MinHeap{

    T* heap, inf;
    int heapsize, maxsize;

    void buildHeap(){
        for(int i=floor(heapsize/2)-1; i>=0; i--){
            minHeapify(i);
        };
    };

    void decreaseKey(int i, T key){
        if (key > heap[i]){
            cout<<"New key is greater than current key"<<endl;
        } else {
            heap[i] = key;
            while(i>0 && heap[parent(i)] > heap[i]){
                swap (heap[i], heap[parent(i)]);
                i = parent(i);
            };
        };
    };

    int parent(int i) {
        return floor((i-1)/2);
    };

    int leftChild(int i) {
        return 2*i+1;
    };

    int rightChild(int i) {
        return 2*i+2;
    };

public:

    MinHeap(vector<T>& vect, T inf = INT32_MAX){
        heap = new T[vect.size()];
        for (int i = 0; i < vect.size(); i++) heap[i] = vect[i];
        heapsize = vect.size();
        maxsize = vect.size();
        this->inf = inf;
        buildHeap();
    };

    MinHeap (T inf = INT32_MAX, int size = 1000000){
        heap = new T[size];
        heapsize = 0;
        maxsize = size;
        this->inf = inf;
        buildHeap();
    };

    void minHeapify(int i){
        int l = leftChild(i);
        int r = rightChild(i);
        int min;
        if (l <= heapsize && heap[l] < heap[i]) min=l;
        else min = i;
        if (r <= heapsize && heap[r] < heap[min]){
            min = r;
        };
        if (min != i) {
            swap (heap[i],heap[min]);
            minHeapify(min);
        };
    };
    
    void insert (T key){
        int k = heapsize;
        heapsize++;
        heap[heapsize-1] = inf;
        decreaseKey(heapsize-1, key);
        //cout << get<1>(key) << " inserted" << endl;
    };

    void deleteKey() {
        if (heapsize < 1){
            cout << "Heap underflow" << endl;
        } else {
            heap[0] = heap[heapsize-1];
            heapsize--;
            minHeapify(0);
        };
    };

    T getMin() {
        return heap[0];
    };

    int getHeapsize() {
        return heapsize;
    };

    T& operator[] (int index) {
        return heap[index];
    };

    T extractMin() {
        T r = getMin();
        deleteKey();
        //cout << get<1>(r) << " " << heapsize << endl;
        return r;
    };

};
