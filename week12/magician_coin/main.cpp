#include <iostream>
#include <vector>
#include <iomanip>

int n, m;
std::vector<double> probabilities;

std::vector<std::vector<double>> memo;

double probability_optimal_strategy(int budget, int day) {
    if (budget >= m) return 1;
    if (day == n)    return 0;
    if (memo[day][budget] >= 0) return memo[day][budget];
    double best = 0;
    for (int b = 0; b <= budget; b++)
    {
        double prob = 0;
        prob += probabilities[day] * probability_optimal_strategy(budget + b, day + 1);
        prob += (1 - probabilities[day]) * probability_optimal_strategy(budget - b, day + 1);
        best = std::max(best, prob);
    }
    memo[day][budget] = best;
    return best;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int k; std::cin >> n >> k >> m;

        probabilities.clear();
        for (size_t i = 0; i < n; i++) {
            double p; std::cin >> p;
            probabilities.push_back(p);
        }

        memo.clear();
        memo.resize(n, std::vector<double>(m, -1));

        std::cout << std::fixed << std::setprecision(5);
        std::cout << probability_optimal_strategy(k, 0) << std::endl;
    }

    return 0;
}
