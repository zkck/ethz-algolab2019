#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <stack>
#include <map>

std::vector<int> ages;
std::vector<std::string> species;

struct query {
    int age_bound;
    size_t query_index;
};
std::vector<std::vector<struct query>> queries;

std::vector<int> query_results;

void DFSResults(std::vector<int> &path,
    std::vector<int> &path_ages,
    int node,
    std::vector<size_t> children[])
{
    path.push_back(node);
    path_ages.push_back(ages[node]);

    // for each query, find the upper bound
    for (struct query &q : queries[node]) {
        auto upper = std::upper_bound(path_ages.rbegin(), path_ages.rend(), q.age_bound);
        int index = upper - path_ages.rbegin() - 1;
        query_results[q.query_index] = *(path.rbegin() + index); // could maybe cause SEGV
    }

    for (size_t child : children[node])
    {
        DFSResults(path, path_ages, child, children);
    }

    path.pop_back();
    path_ages.pop_back();
}


int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int n, q; std::cin >> n >> q;

        ages.clear();
        species.clear();
        int oldest_age = -1;
        int oldest_idx = -1;
        std::map<std::string, size_t> indices;
        for (size_t i = 0; i < n; i++)
        {
            std::string s; int a; std::cin >> s >> a;
            indices.insert(std::make_pair(s, i));
            if (a > oldest_age) {
                oldest_age = a;
                oldest_idx = i;
            }
            ages.push_back(a);
            species.push_back(s);
        }

        std::vector<size_t> children[n];

        for (size_t i = 0; i < n - 1; i++)
        {
            std::string s, p; std::cin >> s >> p;
            size_t s_idx = indices.find(s)->second;
            size_t p_idx = indices.find(p)->second;
            children[p_idx].push_back(s_idx);
        }

        queries.clear();
        queries.resize(n);
        for (size_t i = 0; i < q; i++)
        {
            std::string s; int b; std::cin >> s >> b;
            size_t index = indices.find(s)->second;
            struct query q = { b, i };
            queries[index].push_back(q);
        }

        // IDEA 2: Use a DFS

        query_results.clear();
        query_results.resize(n, -1);

        std::vector<int> path;
        std::vector<int> path_ages;
        DFSResults(path, path_ages, oldest_idx, children);

        for (size_t i = 0; i < q; ++i) {
            int species_index = query_results[i];
            if (species_index < 0)
                std::runtime_error("no result for query");
            else
                std::cout << species[species_index] << (i == q - 1 ? "" : " ");
        }
        std::cout << std::endl;


    }
    return 0;
}
