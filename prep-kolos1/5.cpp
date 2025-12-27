#include <iostream>

// a connection of two nodes (cities?)
struct Edge
{
    int destination;
    int weight;
    Edge* next;
};

int main()
{
    std::ios_base::sync_with_stdio(false);
    std::cout.tie(nullptr);
    std::cin.tie(nullptr);

    int V, E;
    std::cin >> V >> E;

    Edge** graph = new Edge*[V];
    for (int i = 0; i < V; i++)
    {
        graph[i] = nullptr;
    }

    for (int i = 0; i < E; i++)
    {
        int a, b, w;
        std::cin >> a >> b >> w;  // connection from a to b with weight w
        Edge* edge = new Edge{b, w, graph[a]};
        graph[a] = edge;
    }

    // entry cost of node/city
    int* entryCost = new int[V];
    for (int i = 0; i < V; i++)
    {
        std::cin >> entryCost[i];
    }

    // targets to find distance to
    int n;
    std::cin >> n;
    int* targets = new int[n];
    bool* isTarget = new bool[V]();  // track which vertices are targets
    for (int i = 0; i < n; i++)
    {
        std::cin >> targets[i];
        isTarget[targets[i]] = true;
    }
    
    // start
    int s;
    std::cin >> s;
    
    long* dist = new long[V];
    bool* visited = new bool[V];

    /*
        some kind of "dijkstras shortest path algorithm"
        go to the unvisited node with the smallest distance to it
        mark it as visited, look at all outgoing paths
        for each neighbor check if that path is actually cheaper
    */
    // initially everything is placeholder dist (-1) and unvisited
    for (int i = 0; i < V; i++)
    {
        dist[i] = -1;
        visited[i] = false;
    }

    // starting node has distance 0
    dist[s] = 0;
    
    int foundTargets = 0;  // early termination when all targets found
    
    for (int count = 0; count < V; count++)
    {
        // find the unvisited node with the shortest distance
        int u = -1;
        long minDist = -1;

        for (int i = 0; i < V; i++)
        {
            if (!visited[i] && dist[i] >= 0)  // unvisited, reachable
            {
                if (minDist == -1 || dist[i] < minDist)
                {
                    minDist = dist[i];
                    u = i;
                }
            }
        }
        
        // no reachable nodes found
        if (u == -1) break;

        visited[u] = true;
        
        // stop when all targets found
        if (isTarget[u])
        {
            foundTargets++;
            if (foundTargets == n) break;
        }

        // does going through the current node make the path cheaper?
        for (Edge* edge = graph[u]; edge != nullptr; edge = edge->next)
        {
            int v = edge->destination;
            long weight = edge->weight;
            // new distance = current + weight + entry cost
            long newDist = dist[u] + weight + entryCost[v];

            // if the new distance is shorter, remember it
            if (dist[v] == -1 || newDist < dist[v])
            {
                dist[v] = newDist;
            }
        }
    }
    
    for (int i = 0; i < n; i++)
    {
        int target = targets[i];
        if (dist[target] == -1)
        {
            std::cout << "NIE\n";
        }
        else
        {
            std::cout << dist[target] << "\n";
        }
    }
    
    for (int i = 0; i < V; i++)
    {
        Edge* edge = graph[i];
        while (edge != nullptr)
        {
            Edge* next = edge->next;
            delete edge;
            edge = next;
        }
    }
    delete[] graph;
    delete[] entryCost;
    delete[] targets;
    delete[] isTarget;
    delete[] dist;
    delete[] visited;
}