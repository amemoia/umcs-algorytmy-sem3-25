#include <iostream>
using namespace std;

/*
Kolejka prioretytowa: Max-Heap
The parent is always greater or equal to its children
for element with index i:
Left child: 2i + 1
Right child: 2i + 2
Parent: (i - 1) / 2
*/
int top(int t[],int n) {
    return t[0];
}
void pop(int t[], int &n) {
    t[0] = t[n-1];
    n--;
    // heapify
    int i = 0;
    while (true) {
        int left = 2*i+1;
        int right = 2*i+2;
        int largest = i;
        
        if (left < n && t[largest] < t[left]) largest = left;
        if (right < n && t[largest] < t[right]) largest = right;

        if (largest != i) {
            int temp = t[i];
            t[i] = t[largest];
            t[largest] = temp;
            i = largest;
        }
        else break;
    }
}
void push(int t[], int &n, int e) {
    t[n] = e;
    int i = n;
    n++;
    //heap up
    while (i > 0) {
        int parent = (i-1)/2;
        if (t[i] > t[parent]) {
            int temp = t[parent];
            t[parent] = t[i];
            t[i] = temp;
            i = parent;
        }
        else break;
    }
    
}