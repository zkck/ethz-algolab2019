#include <iostream>
#include <vector>
#include <queue>

// memo
std::vector<int> memo;

// as per tree modeling of the problem
std::vector<std::vector<int>> children;

// costs of rebuilding a city
std::vector<int> costs;

int selecting_cost(int i) {
    // if (memo[i] > 0) return memo[i];
    int result = costs[i];
    for (int c1 : children[i])
        for (int c2 : children[c1])
            result += selecting_cost(c2);
    // memo[i] = result;
    return result;
}

int main(int argc, char const *argv[])
{
    int num_tests;
    std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        // get number of cities
        int n; std::cin >> n;

        // adjacency information
        std::vector<int> adj[n];

        for (size_t stage = 0; stage < n - 1; stage++)
        {
            int i, j; std::cin >> i >> j;
            adj[i].push_back(j);
            adj[j].push_back(i);
        }

        costs.clear();
        costs.resize(n);
        
        for (size_t i = 0; i < n; i++)
        {
            int c; std::cin >> c;
            costs.push_back(c);
        }



        // step 1, build the tree

        children.clear();
        children.resize(n);
        
        std::queue<int> Q;
        std::vector<bool> visited(n, false);
        Q.push(0);
        visited[0] = true;
        while (!Q.empty()) {
            int v = Q.front(); Q.pop();
            for (int u : adj[v]) {
                if (!visited[u]) {
                    children[v].push_back(u);
                    Q.push(u);
                    visited[u] = true;
                }
            }
        }

        // step 2
        int selecting = selecting_cost(0);
        int not_selecting = 0;
        for (int c : children[0])
            not_selecting += selecting_cost(c);

        std::cout << std::min(selecting, not_selecting) << std::endl;
        
    }
    
    return 0;
}
