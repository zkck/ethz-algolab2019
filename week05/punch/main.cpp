#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

struct beverage {
    int cost;
    int volume;
};

int n, k;
std::vector<beverage> beverages;

std::vector<std::vector<int>> memo;
double cheapest(int i, int ppl) {
    if (ppl >= k)   return 0;
    if (i == n)     return std::numeric_limits<double>::infinity();
    if (memo[i][ppl] >= 0) return memo[i][ppl];
    double result = cheapest(i, ppl + beverages[i].volume) + beverages[i].cost;
    result = std::min(result, cheapest(i + 1, ppl));
    memo[i][ppl] = result;
    return result;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        std::cin >> n >> k;

        // beverage is a (cost, volume) pair
        beverages.clear();
        beverages.reserve(n);

        for (size_t i = 0; i < n; i++)
        {
            struct beverage beverage;
            std::cin >> beverage.cost >> beverage.volume;
            beverages.push_back(beverage);
        }

        memo.clear();
        memo.resize(n, std::vector<int>(k, -1));
        std::cout << (int) cheapest(0, 0) << std::endl;

    }

    return 0;
}
