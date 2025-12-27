#include <iostream>

int* parents;
// tracks how many connections a node has
int* ranks;

// find parent of x
int find(int x)
{
    if (parents[x] != x) parents[x] = find(parents[x]);
    return parents[x];
}

// the root with more connections becomes the new parent (or parentX if equal)
void connect(int x, int y)
{
    int parentX = find(x);
    int parentY = find(y);

    if (parentX != parentY)
    {
        if (ranks[parentX] < ranks[parentY])
        {
            parents[parentX] = parentY;
        }
        else if (ranks[parentX] > ranks[parentY])
        {
            parents[parentY] = parentX;
        }
        else
        {
            parents[parentY] = parentX;
            ranks[parentX]++;
        }
    }
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int V, E;
    std::cin >> V >> E;

    parents = new int[V];
    ranks = new int[V];

    for (int i = 0; i < V; i++)
    {
        parents[i] = i;
        ranks[i] = 0;
    }
    for (int i = 0; i < E; i++)
    {
        int a, b;
        std::cin >> a >> b;
        connect(a, b);
    }

    int groups = 0;
    for (int i = 0; i < V; i++) if (find(i) == i) groups++;
    // to connect x groups we need k-1 connections
    std::cout << groups - 1 << std::endl;

    delete[] parents;
    delete[] ranks;
}
