#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

struct beverage {
    int cost;
    int volume;
};

struct result {
    double price;
    int num_distinct;
};

struct result best_result(struct result r1, struct result r2) {
    if (r1.price == r2.price) {
        return r1.num_distinct > r2.num_distinct ? r1 : r2;
    } else {
        return r1.price < r2.price ? r1 : r2;
    }
}

int equal(struct result r1, struct result r2) {
    return r1.price == r2.price && r1.num_distinct == r1.num_distinct;
}

int n, k;
std::vector<beverage> beverages;

const struct result INF     = { std::numeric_limits<double>::infinity(),  0 };
const struct result ZERO    = { 0,  0 };
const struct result UNKNOWN = { 0, -1 };

std::vector<std::vector<struct result>> memo;
std::vector<std::vector<struct result>> fresh_memo;
struct result cheapest(int i, int ppl, int fresh) {
    if (ppl >= k)   return ZERO;
    if (i == n)     return INF;

    if (!fresh && !equal(memo[i][ppl], UNKNOWN)) return memo[i][ppl];
    if (fresh && !equal(fresh_memo[i][ppl], UNKNOWN)) return fresh_memo[i][ppl];

    struct result with = cheapest(i, ppl + beverages[i].volume, false);
    with.price += beverages[i].cost;
    with.num_distinct += fresh ? 1 : 0;

    struct result without = cheapest(i + 1, ppl, true);

    struct result result = best_result(with, without);
    if (!fresh) memo[i][ppl] = result;
    else fresh_memo[i][ppl] = result;

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
        memo.resize(n, std::vector<struct result>(k, UNKNOWN));
        fresh_memo.clear();
        fresh_memo.resize(n, std::vector<struct result>(k, UNKNOWN));
        auto result = cheapest(0, 0, true);
        std::cout << (int) result.price << " " << result.num_distinct << std::endl;

    }

    return 0;
}
