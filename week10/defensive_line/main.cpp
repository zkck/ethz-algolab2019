#include <iostream>
#include <vector>
#include <limits>

#define ILLEGAL -std::numeric_limits<double>::infinity()

int n, m;

std::vector<int> jumps;
std::vector<std::vector<double>> memo;

double DP(int defender, int attacker) {
    if (attacker == m) return 0;        // no more attackers left, legal
    if (defender == n) return ILLEGAL;  // reached last defender with attackers left, illegal
    if (memo[attacker][defender] != -1) return memo[attacker][defender];
    double result = DP(defender + 1, attacker);
    if (jumps[defender] >= 0) {
        int reward = jumps[defender] - defender + 1;
        result = std::max(result, reward + DP(jumps[defender] + 1, attacker + 1));
    }
    memo[attacker][defender] = result;
    return result;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int k; std::cin >> n >> m >> k;

        int partial_sums[n];

        int sum = 0;
        for (size_t i = 0; i < n; i++)
        {
            int v; std::cin >> v;
            sum += v;
            partial_sums[i] = sum;
        }

        jumps.clear();
        jumps.resize(n, -1);

        size_t left = 0, right = 0;
        for (; right < n; right++)
        {
            // decrease until something, meaning increasing left
            int p_sum = 0;
            do {
                int l_sum = left == 0 ? 0 : partial_sums[left - 1];
                int r_sum = partial_sums[right];
                p_sum = r_sum - l_sum;
            } while (p_sum > k && ++left <= right); // lazy evaluation makes shit work nicely here
            if (p_sum == k) {
                // std::cout << left << " " << right << std::endl;
                jumps[left] = right;
            }
        }

        memo.clear();
        memo.resize(m);
        for (auto &vector : memo) vector.resize(n, -1);

        double result = DP(0, 0);
        if (result == ILLEGAL)
            std::cout << "fail" << std::endl;
        else
            std::cout << (int) result << std::endl;
    }

    return 0;
}
