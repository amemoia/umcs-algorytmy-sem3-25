#include <iostream>

// min-heap functions
// n is the current array size
void pop(long long t[], int &n) {
    t[0] = t[n-1];
    n--;
    // heapify
    int i = 0;
    while (true) {
        int left = 2*i+1;
        int right = 2*i+2;
        int smallest = i;
        
        if (left < n && t[smallest] > t[left]) smallest = left;
        if (right < n && t[smallest] > t[right]) smallest = right;

        if (smallest != i) {
            int temp = t[i];
            t[i] = t[smallest];
            t[smallest] = temp;
            i = smallest;
        }
        else break;
    }
}

void push(long long t[], int &n, long long e) {
    t[n] = e;
    int i = n;
    n++;
    //heap up
    while (i > 0) {
        int parent = (i-1)/2;
        if (t[i] < t[parent]) {
            int temp = t[parent];
            t[parent] = t[i];
            t[i] = temp;
            i = parent;
        }
        else break;
    }
    
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);
    int N;
    std::cin >> N;
    long long *values = new long long[N];
    long long stability = 0;
    int count = 0;

    for (int i = 0; i < N; i++) {
        long long vi;
        std::cin >> vi;
        push(values, count, vi);
        stability+=vi;
        //count++;

        if (stability < 0) {
            stability -= values[0]; // always the smallest
            pop(values, count);
            //count--;
        }
    }

    std::cout << count << '\n';
    delete[] values;
}