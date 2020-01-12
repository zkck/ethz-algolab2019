#include <iostream>
#include <vector>
#include <algorithm>

std::vector<std::vector<int>> memo;
std::vector<std::vector<int>> p;

int earliest_finish(int i);
int earliest_finish(int i, int j);

int earliest_finish(int i)
{
    int result = std::numeric_limits<int>::max();
    for (size_t j = 0; j < p[i].size(); j++)
    {
        result = std::min(result, earliest_finish(i, j) - p[i][j]);
    }
    return result;
}

int earliest_finish(int i, int j)
{
    if (memo[i][j] >= 0) return memo[i][j];
    if (i == p.size() - 1) return p[i][j];

    int result = std::numeric_limits<int>::max();

    auto upper = std::upper_bound(p[i + 1].begin(), p[i + 1].end(), p[i][j]);
    if (upper != p[i + 1].end())
        result = std::min(result, earliest_finish(i + 1, upper - p[i + 1].begin()));

    memo[i][j] = result;
    return result;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;

        int m[n];
        for (size_t i = 0; i < n; i++) {
            std::cin >> m[i];
        }

        memo.clear();
        memo.resize(n);

        p.clear();
        p.resize(n);

        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < m[i]; j++) {
                int p_i_j; std::cin >> p_i_j;
                p[i].push_back(p_i_j);
            }
            std::sort(p[i].begin(), p[i].end());
            memo[i].resize(m[i], -1);
        }

        std::cout << earliest_finish(0) << std::endl;


    }

    return 0;
}
