#include <iostream>
#include <vector>

int binary_count(int i, int N) {
    int count = 0;
    for (size_t idx = 0; idx < N; ++idx) {
        if (!(i & 1<<idx)) count++;
    }
    return count;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int N, M; std::cin >> N >> M;

        int b[M];
        for (size_t i = 0; i < M; i++)
            std::cin >> b[i];

        int on[M][N], off[M][N];
        for (size_t i = 0; i < N; i++)
            for (size_t j = 0; j < M; j++)
                std::cin >> on[j][i] >> off[j][i];

        std::vector<int> subsets;
        for (int subset = 0; subset < 1<<N; subset++) {
            int correct = true;
            for (size_t i = 0; i < M && correct; i++) {
                int sum = 0;
                for (size_t j = 0; j < N && sum <= b[i]; j++) {
                    if (subset & 1<<j) sum += on[i][j];
                    else               sum += off[i][j];
                }
                if (sum != b[i]) correct = false;
            }
            if (correct) subsets.push_back(subset);
        }

        int min_length = N;
        for (int ss : subsets) {
            int count = binary_count(ss, N);
            if (count < min_length)
                min_length = count;
        }

        if (subsets.empty())
            std::cout << "impossible" << std::endl;
        else
            std::cout << min_length << std::endl;
    }

    return 0;
}
