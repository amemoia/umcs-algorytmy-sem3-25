#include <iostream>

struct Pair
{
    int a;
    int b;
    int i;
};

void sort(Pair* pairs, int start, int end)
{
    for (int i = start+1; i < end; i++)
    {
        Pair key = pairs[i];
        int j = i-1;
        while (j>= start && (pairs[j].a > key.a) || (pairs[j].a == key.a && pairs[j].i > key.i))
        {
            pairs[j+1] = pairs[j];
            j--;
        }
        pairs[j+1] = key;
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int n;
    std::cin >> n;
    Pair* pairs = new Pair[n];

    int len = 0;
    for (int i = 0; i < n; i++)
    {
        std::cin >> pairs[i].a >> pairs[i].b;
        pairs[i].i = i;
    }

    int evens_begin = 0;
    for (int i = 0; i < n; i++)
    {
        if (pairs[i].a % 2 == 0)
        {
            Pair temp = pairs[i];
            for (int j = i; j > evens_begin; j--)
            {
                pairs[j] = pairs[j-1];
            }
            pairs[evens_begin] = temp;
            evens_begin++;
        }
    }

    // pass 1: evens
    sort(pairs, 0, evens_begin);
    // pars 2: odds come after evens
    sort(pairs, evens_begin, n);

    for (int i = 0; i < n; i++)
    {
        std::cout << pairs[i].b << ' ';
    }
    std::cout << '\n';
    delete[] pairs;
}
