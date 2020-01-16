#include <iostream>
#include <vector>
#include <queue>

#define LEVEL_END -1

int main(int argc, char const *argv[])
{
    // IDEA 1 : Bottom up DP using BFS

    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        long x;
        int n, m, k;
        std::cin >> n >> m >> x >> k;

        // int points[n][n] = { 0 };
        std::vector<int> points[n];
        for (size_t i = 0; i < n; i++)
        {
            points[i].resize(n, 0);
        }


        std::vector<int> adj[n];
        for (size_t i = 0; i < m; i++)
        {
            int u, v, p; std::cin >> u >> v >> p;
            adj[u].push_back(v);
            points[u][v] = std::max(points[u][v], p);
        }

        // BFS

        std::vector<long> scores[k + 1];
        for (size_t i = 0; i <= k; i++)
            scores[i].resize(n, -1);

        std::queue<int> Q;
        std::queue<int> nextQ;

        // Modify BFS queue to include levels: each end of level involves a -1
        Q.push(0);

        scores[0][0] = 0;

        int level = 0;
        while (level < k)
        {
            if (Q.empty()) {
                Q = nextQ;
                nextQ = std::queue<int>();
                level++;
                continue;
            }

            int u = Q.front(); Q.pop();

            for (int v : adj[u]) {
                if (scores[level + 1][v] < 0) nextQ.push(v);
                long s = scores[level][u] + points[u][v];
                scores[level + 1][v] = std::max(scores[level + 1][v], s);
                // printf("  score=%ld for %d->%d\n", s, u, v);
            }
        }

        // Find the earliest k that goes past x
        int found = false;
        for (size_t i = 0; i <= k && !found; i++) {
            for (long score : scores[i]) {
                if (score >= x) {
                    found = true;
                    std::cout << i << std::endl;
                    break;
                }
            }
        }

        if (!found) std::cout << "Impossible" << std::endl;

    }

    return 0;
}
