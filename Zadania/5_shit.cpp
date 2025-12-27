#include <iostream>

// sort of how we made template classes during java class
// this can be reused in pretty much any (?) task :steamhappy:
template <typename T>
class ExtArray
{
public:
    int len;
    int capacity;
    T* data;

     ExtArray() { len = 0; capacity = 16; data = new T[capacity]; }
    ~ExtArray() { delete[] data; }

    void resize()
    {
        capacity *= 2;
        T* temp = new T[capacity];
        for (int i = 0; i < len; i++)
        {
            temp[i] = data[i];
        }
        delete[] data;
        data = temp;
    }

    void add(const T& value)
    {
        if (len == capacity) resize();
        data[len++] = value;
    }

    T& operator[](int index) { return data[index]; }
};

// again, we're doing a template for reuse later
template <typename T>
class Queue
{
public:
    T* data;
    int front;
    int back;
    int capacity;

    Queue(int size)
    {
        front = 0;
        back = 0;
        capacity = size;
        data = new T[capacity];
    }

    ~Queue() { delete[] data; }

    T pop() { return data[front++]; }
    void push(T val) { data[back++] = val; }
    bool empty() { return front >= back; }

    void reset()
    {
        front = 0;
        back = 0;
    }
};

// THIS IS SO OVERENGINEERED BUT I WANTED IT TO BE REUSABLE WAHHHH
class Graph
{
public:
    // total count of nodes
    int n;
    // ExtArray of neighbors for Node i
    ExtArray<ExtArray<int>>* neighbors;
    // was Node i already checked?
    ExtArray<int>* visited;
    // height for Node i
    ExtArray<int>* heights;

    Graph(int n)
    {
        this->n = n;
        neighbors = new ExtArray<ExtArray<int>>();
        visited = new ExtArray<int>();
        heights = new ExtArray<int>();

        for (int i = 0; i < n; i++)
        {
            neighbors->add(ExtArray<int>());
            visited->add(0);
        }
    }

    ~Graph()
    {
        delete neighbors;
        delete visited;
        delete heights;
    }
};

inline int my_min(int a, int b) { return (a < b) ? a : b; }

bool bfs(Graph* graph, int start, int end, int n, int current_water_level, int& min_height, int loop)
{
    // deeply evil approach for code readability
    ExtArray<ExtArray<int>>& neighbors = *(graph->neighbors);
    ExtArray<int>& visited = *(graph->visited);
    ExtArray<int>& heights = *(graph->heights);

    // is start or end flooded?
    // does start even have any connections?
    // is start the same as end?
    if (heights[start] < current_water_level
        || heights[end] < current_water_level
        || neighbors[start].len == 0)
        return false;
    if (start == end) {
        min_height = my_min(heights[start], heights[end]);
        return true;
    }

    Queue<int> q(n);
    q.push(start);
    visited[start] = loop;
    int min_height_encountered = heights[start];

    while (!q.empty())
    {
        int current = q.pop();
        min_height_encountered = my_min(min_height_encountered, heights[current]);
    
        for (int i = 0; i < neighbors[current].len; i++)
        {
            int neighbor = neighbors[current][i];
            // check if not visited in this loop and not flooded
            if (visited[neighbor] < loop && heights[neighbor] >= current_water_level)
            {
                if (neighbor == end) { 
                    min_height = my_min(min_height_encountered, heights[neighbor]);
                    return true; 
                }
                visited[neighbor] = loop;
                q.push(neighbor);
            }
        }
    }

    return false;
}

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int n, m;
    std::cin >> n >> m;

    Graph* graph = new Graph(n);
    ExtArray<ExtArray<int>> neighbors = *(graph->neighbors);
    ExtArray<int> visited = *(graph->visited);
    ExtArray<int> heights = *(graph->heights);

    for (int i = 0; i < n; i++)
    {
        int height;
        std::cin >> height;
        heights.add(height);
    }

    // node connections
    for (int i = 0; i < m; i++)
    {
        int u, v;
        std::cin >> u >> v;
        neighbors[u].add(v);
        neighbors[v].add(u);
    }

    int start, end;
    std::cin >> start >> end;

    // water levle changes
    int w;
    std::cin >> w;

    int current_water_level = 0;
    int min_height = -1;
    bool prev_possible = false;

    for (int i = 0; i < w; i++)
    {
        int change;
        std::cin >> change;

        current_water_level += change;

        // if the water level now is higher than the minimum height of the previous path, skip bfs
        if (prev_possible && min_height >= 0 && current_water_level > min_height)
        {
            std::cout << "NIE\n";
            prev_possible = false;
            continue;
        }
        prev_possible = bfs(graph, start, end, n, current_water_level, min_height, i+1);
        if (prev_possible)
        {
            std::cout << "TAK\n";
        }
        else
        {
            std::cout << "NIE\n";
        }
    }

    delete graph;
}
