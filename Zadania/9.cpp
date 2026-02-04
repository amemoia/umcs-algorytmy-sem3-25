/*
 *  in: long (bytes)
 *  divided into z_1...z_7, each z_x is a byte (0...255)
 *  to get specific z_k:
 *  (1234567890 / 256^k) % 256
 *  interesuje nas 0, 4, 7
 *
 *  sort order:
 *      FIRST              z4 ascending
 *      IF z4 EQUAL:       z7 ascending
 *      IF z7 ALSO EQUAL:  z0 ascending
 *
 *  linear sort (radix sort)
 *
 */

#include <iostream>

int main()
{
    std::ios::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    unsigned int n;
    std::cin >> n;
    //n = 3;
    unsigned long* nums = new unsigned long[n];
    unsigned long* nums_sorted = new unsigned long[n];
    //long nums[10] = {243, 123, 65, 527, 11, 12, 451, 627, 100, 231};
    //long nums_sorted[10];

    int base = 256;
    int keys[3] = {0, 7, 4};  // important: ordered least important to most important
    int keys_count = 3;

    for (unsigned int i = 0; i < n; i++)
    {
        std::cin >> nums[i];
    }

    // optimization: instead of going over each digit we count its occurences
    // i guess this could be an int?
    unsigned long* count = new unsigned long[base];
    // cyfra
    for (int i = 0; i < keys_count; i++)
    {
        int key = keys[i];
        long div = 1;
        for (int m = 0; m < key; m++) div*=base;

        // reset counts
        for (int j = 0; j < base; j++) count[j] = 0;
        // value
        for (unsigned int j = 0; j < n; j++)
        {
            unsigned long digit = (nums[j] / div) % base;
            count[digit]++;
        }
        for (int j = 1; j < base; j++) count[j] += count[j-1];


        // places elements backwards to keep the sorting stable
        for (unsigned int j = n; j > 0; j--)
        {
            unsigned long digit = (nums[j-1] / div) % base;
            nums_sorted[--count[digit]] = nums[j-1];
        }

        for (unsigned int j = 0; j < n; j++) nums[j] = nums_sorted[j];
    }

    unsigned long result = 0;
    unsigned long mod = 1000000007;
    // SUM (PosortowaneIDi XOR i) % (10^9 + 7)
    for (unsigned int i = 0; i < n; i++)
    {
        //std::cout << nums[i] << '\n';
        result += (nums[i] ^ (i+1)) % mod;
    }
    std::cout << (result % mod) << '\n';
    delete[] nums;
    delete[] nums_sorted;
    delete[] count;
}