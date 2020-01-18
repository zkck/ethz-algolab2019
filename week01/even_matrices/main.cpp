#include <iostream>

int algo()
{
    int n; std::cin >> n;
    int M[n][n];
    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < n; j++)
            std::cin >> M[i][j];

    int count = 0;
    for (size_t iteration = 0; iteration < n; iteration++)
    {
        // Sub-matrix, which is shifted down: which means when accessing M, the row-delta would
        // be iteration
        int ps[n - iteration][n];

        // Run partial sums on the sub-matrix
        for (size_t i = 0; i < n - iteration; i++)
        {
            int count_even = 0;
            int count_odd  = 0;
            for (size_t j = 0; j < n; j++)
            {
                int ps_up   = i == 0 ? 0 : ps[i - 1][j];
                int ps_left = j == 0 ? 0 : ps[i][j - 1];

                int ps_upleft = i == 0 || j == 0 ? 0 : ps[i - 1][j - 1];

                ps[i][j] = ps_up + ps_left - ps_upleft + M[i + iteration][j];

                if (ps[i][j] % 2 == 0)
                    count_even++;
                else
                    count_odd++;
            }
            count += count_even;
            count += count_odd * (count_odd - 1) / 2;
            count += count_even * (count_even - 1) / 2;
        }

    }

    return count;
}


int main(int argc, char const *argv[]) {
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) std::cout << algo() << std::endl;
    return 0;
}