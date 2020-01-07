#include <iostream>
#include <vector>


std::vector<int> coins;
std::vector<std::vector<int>> memo;

int max_gain(bool turn, int l, int r) {
    if (l == r)
        return turn ? coins[l] : 0;

    if (memo[l][r] != -1)
        return memo[l][r];

    int res;
    if (turn) {
        int max_l = coins[l] + max_gain(!turn, l + 1, r);
        int max_r = coins[r] + max_gain(!turn, l, r - 1);
        res = max_l > max_r ? max_l : max_r;
    } else {
        int max_l = max_gain(!turn, l + 1, r);
        int max_r = max_gain(!turn, l, r - 1);
        res = max_l > max_r ? max_r : max_l;
    }

    memo[l][r] = res;
    return res;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        coins.clear();
        memo.clear();

        int n; std::cin >> n;
        for (int i = 0; i < n; ++i) {
            int elem; std::cin >> elem;
            coins.push_back(elem);
            memo.push_back(std::vector<int>(n, -1));
        }

        std::cout << max_gain(true, 0, n - 1) << std::endl;
    }
    return 0;
}
