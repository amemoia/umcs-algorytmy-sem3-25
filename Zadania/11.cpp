#include <iostream>

// quickselect robi swap tak jak quicksort, tylko przestaje jeśli szukana liczba jest już na pozycji k
unsigned int quickselect(unsigned int arr[], int left, int right, int k) {
    while (left <= right) {
        // partition
        int mid = left + (right - left) / 2;
        unsigned int pivot = arr[mid];
        std::swap(arr[mid], arr[right]);

        int i = left;
        for (int j = left; j < right; j++) {
            if (arr[j] < pivot) {
                std::swap(arr[i], arr[j]);
                i++;
            }
        }
        std::swap(arr[i], arr[right]);

        unsigned int pivot_i = i;

        if (pivot_i == k) return arr[pivot_i];
        else if (pivot_i > k) right = pivot_i - 1;
        else left = pivot_i + 1; 
    }
    return 0;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int z;
    std::cin >> z;
    int *keys = new int[z];
    unsigned int *valid_hints = new unsigned int[z];
    for (int i = 0; i < z; i++) std::cin >> keys[i];

    for (int i = 0; i < z; i++) {
        int ni;
        std::cin >> ni;
        unsigned int *hints = new unsigned int[ni];
        for (int j = 0; j < ni; j++) std::cin >> hints[j];
        valid_hints[i] = quickselect(hints, 0, ni-1, keys[i]-1);
        delete[] hints;
    }

    for (int i = 0; i < z; i++) {
        unsigned int cur = valid_hints[i];
        while (cur > 0) {
            char byte = cur & 0xFF;
            std::cout << byte;
            cur = cur >> 8;
        }
    }
    delete[] keys;
    delete[] valid_hints;
}