#include <iostream>
#include <vector>
#include <limits>

std::vector<std::vector<int>> edges;
std::vector<std::vector<int>> points;

std::vector<int> best_remaining;

int max_points(int count, int vertex, int remaining) {
    if (remaining < 0) {
        return count;
    }
    int max = std::numeric_limits<int>::min();
    for (int neighbor : edges[vertex]) {
        if (!visited[neighbor])
    }
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        edges.clear();
        visited.clear();
        max_points.clear();

        int n, m, x, k; std::cin >> n >> m >> x >> k;
        visited = std::vector<bool>(n, false);
        edges = std::vector<std::vector<int>>(n);
        points = std::vector<std::vector<int>>(n, std::vector<int>(n, -1));


        for (size_t i = 0; i < m; i++)
        {
            int u, v, p; std::cin >> u >> v >> p;
            edges[u].push_back(v);
            points[u][v] = p;
        }


        
    }
    
    return 0;
}
