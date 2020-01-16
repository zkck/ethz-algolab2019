#include <iostream>
#include <vector>
#include <limits>

std::vector<int> psums_a, psums_b;

int min_cost(std::vector<int> memo[], int A, int B) {
    if (A == 1 || B == 1) return (psums_a[A - 1] - A) * (psums_b[B - 1] - B);
    if (memo[A - 1][B - 1] >= 0) return memo[A - 1][B - 1];
    int result = (psums_a[A - 1] - A) * (psums_b[B - 1] - B);
    for (size_t i = 1; i < A; i++) {
        for (size_t j = 1; j < B; j++) {
            int costA = (psums_a[A - 1] - psums_a[i - 1] - A + i);
            int costB = (psums_b[B - 1] - psums_b[j - 1] - B + j);
            result = std::min(result, costA * costB + min_cost(memo, i, j));
        }
    }
    memo[A - 1][B - 1] = result;
    return result;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int n; std::cin >> n;

        int sum;

        // parse A stack
        psums_a.clear();

        sum = 0;
        for (size_t i = 0; i < n; i++) {
            int a; std::cin >> a;
            sum += a;
            psums_a.push_back(sum);
        }

        // parse B stack
        psums_b.clear();

        sum = 0;
        for (size_t i = 0; i < n; i++) {
            int b; std::cin >> b;
            sum += b;
            psums_b.push_back(sum);
        }

        std::vector<int> memo[n];
        for (size_t i = 0; i < n; i++)
            memo[i].resize(n, -1);

        std::cout << min_cost(memo, n, n) << std::endl;
    }
    return 0;
}
