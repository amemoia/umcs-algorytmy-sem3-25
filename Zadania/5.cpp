#include <iostream>

// this is evil chungus i hate it.
// please speed i need this

class Graph
{
public:
    int n;

    int* heights;
    int* neighbor_bounds;
    int* neighbors;

    Graph(int n, int m)
    {
        this->n = n;
        heights = new int[n];
        neighbor_bounds = new int[n + 1];
        neighbors = new int[2 * m];
    }

    ~Graph()
    {
        delete[] heights;
        delete[] neighbor_bounds;
        delete[] neighbors;
    }
};

inline int my_min(int a, int b) { return (a < b) ? a : b; }

// i'm pretty sure most people implemented BFS (Breath-First-Search)
// we pass the visited and queue arrays as arguments to reuse them
// apparently using a bool array due to the resets it takes
// instead, we use ints and increment them, so for example
// if im on loop 5 and the value is less than 5, it means i have not visited it yet
bool BFS(Graph& graph, int start, int end, int* visited, int* queue, int* parent)
{
    // simple queue fine shyt
    int q_head = 0, q_tail = 0;
    queue[q_tail++] = start;
    visited[start] = 1;

    while (q_head < q_tail)
    {
        int cur = queue[q_head++];
        
        if (cur == end)
        {
            return true;
        }

        int n_start = graph.neighbor_bounds[cur];
        int n_end = graph.neighbor_bounds[cur + 1];
        for (int index = n_start; index < n_end; ++index)
        {
            int neighbor = graph.neighbors[index];

            if (visited[neighbor] == 1) continue;

            visited[neighbor] = 1;
            parent[neighbor] = cur;
            queue[q_tail++] = neighbor;
        }
    }
    return false;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    Graph graph(n, m);

    for (int i = 0; i < n; ++i) std::cin >> graph.heights[i];

    // number of neighbors for a node (apparently this is also called its "degree")
    // turns out the () initializes everything with a 0 instead of random values
    int* n_counts = new int[n]();
    // store connections (u,v pairs)
    int* cu = new int[m];
    int* cv = new int[m];

    for (int i = 0; i < m; i++)
    {
        std::cin >> cu[i] >> cv[i];
        ++n_counts[cu[i]];
        ++n_counts[cv[i]];
    }

    /*
        i am pretty much guaranteed to forget this so ill write it down here
        since we are storing all neighbors in one array, for each node we need to know
        where its neighbors start and end (the end being exclusive)
        this is what neighbor_bounds stores
        after that, for each (u, v) pair we put u in v's list and v in u's list.
    */
    graph.neighbor_bounds[0] = 0;
    for (int i = 0; i < n; i++)
    {
        graph.neighbor_bounds[i + 1] = graph.neighbor_bounds[i] + n_counts[i];
    }

    /*
        cursor tracks where to insert the next neighbor for each node
        it's basically how many neighbors we've written to a node
    */
    int* cursor = new int[n]();
    for (int i = 0; i < m; i++)
    {
        int u = cu[i];
        int v = cv[i];
        graph.neighbors[graph.neighbor_bounds[u] + cursor[u]++] = v;
        graph.neighbors[graph.neighbor_bounds[v] + cursor[v]++] = u;
    }

    delete[] cv;
    delete[] cu;
    delete[] n_counts;
    delete[] cursor;

    int start, end;
    std::cin >> start >> end;

    int* visited = new int[n]();
    int* queue = new int[n];
    int* parent = new int[n];
    
    for (int i = 0; i < n; ++i)
    {
        parent[i] = -1;
    }

    bool found = BFS(graph, start, end, visited, queue, parent);
    
    int path_min_height = -1;
    if (found)
    {
        path_min_height = graph.heights[end];
        int curr = end;
        while (curr != -1)
        {
            path_min_height = my_min(path_min_height, graph.heights[curr]);
            curr = parent[curr];
        }
    }

    // water level changes
    int w;
    std::cin >> w;

    long long cur_water_lvl = 0;

    for (int i = 0; i < w; ++i)
    {
        long long change;
        std::cin >> change;
        cur_water_lvl += change;

        if (path_min_height >= cur_water_lvl)
        {
            std::cout << "TAK\n";
        }
        else
        {
            std::cout << "NIE\n";
        }
    }

    delete[] visited;
    delete[] queue;
    delete[] parent;
}
