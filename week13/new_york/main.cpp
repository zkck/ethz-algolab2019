#include <iostream>
#include <deque>
#include <set>
#include <vector>
#include <cassert>

int n, m, k;

// heats
std::vector<int> h;

// tree
std::vector<std::vector<int>> children;

// critical points
std::set<int> critical_points;

void NYDFS(int node, int l, std::multiset<int> &heats_along_path, std::deque<int> &path) {
    path.push_back(node);
    heats_along_path.insert(h[node]);
    if (l == m - 1) {
        int first = path.front();
        int max = *heats_along_path.rbegin();
        int min = *heats_along_path.begin();
        if (max - min <= k)
            critical_points.insert(first);

        // Pull chain down
        path.pop_front();
        auto it = heats_along_path.find(h[first]);
        heats_along_path.erase(it);
        for (int child : children[node]) {
            NYDFS(child, l /* <-- notice it is unchanged */ , heats_along_path, path);
        }
        path.push_front(first);
        heats_along_path.insert(h[first]);

    } else {
        for (int child : children[node]) {
            NYDFS(child, l + 1, heats_along_path, path);
        }
    }
    assert(path.back() == node);
    path.pop_back();
    auto it = heats_along_path.find(h[node]);
    assert(it != heats_along_path.end());
    heats_along_path.erase(it);
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        std::cin >> n >> m >> k;

        h.clear();
        for (size_t i = 0; i < n; i++)
        {
            int heat; std::cin >> heat;
            h.push_back(heat);
        }

        children.clear();
        children.resize(n);
        for (size_t i = 0; i < n - 1; i++)
        {
            int u, v; std::cin >> u >> v;
            children[u].push_back(v);
        }

        critical_points.clear();
        std::multiset<int> heats_along_path;
        std::deque<int> path;
        NYDFS(0, 0, heats_along_path, path);

        if (!critical_points.empty()) {
            for (int cp : critical_points)
                std::cout << cp << " ";
            std::cout << std::endl;
        } else {
            std::cout << "Abort mission" << std::endl;
        }

    }

    return 0;
}
