#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

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

        // IDEA
        //
        // For each room find the satisfying subsets, using split and list. Then move to
        // the next room and from those satisfying subsets,

        // high     |      low
        //
        // ---L2--- | ---L1---

        // L1 is first N / 2 numbers
        const int L1 = N / 2;

        // L2 is remaining numbers
        const int L2 = N - L1;

        std::vector<int> satisfying_subsets;

        if (M == 1) {

            size_t i = 0;

            // Section 1: create the set to compare to
            std::multimap<long, int> L2set;
            for (int subset = 0; subset < 1<<L2; subset++) {
                int sum = 0;
                for (size_t j = 0; j < L2 && sum <= b[i]; j++) {
                    if (subset & 1<<j) sum += on[i][j + L1];
                    else               sum += off[i][j + L1];
                }
                if (sum <= b[i])
                    L2set.insert(std::make_pair(sum, subset << L1));
            }

            // Section 2
            for (int subset = 0; subset < 1<<L1; subset++) {
                int sum = 0;
                for (size_t j = 0; j < L1 && sum <= b[i]; j++) {
                    if (subset & 1<<j) sum += on[i][j];
                    else               sum += off[i][j];
                }
                if (sum <= b[i]) {
                    auto range = L2set.equal_range(b[i] - sum);
                    for (auto it = range.first; it != range.second; it++) {
                        satisfying_subsets.push_back(subset | it->second);
                    }
                }
            }

        } else {

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
                if (correct) satisfying_subsets.push_back(subset);
            }
        }

        int min_length = N;
        for (int ss : satisfying_subsets) {
            int count = binary_count(ss, N);
            if (count < min_length)
                min_length = count;
        }

        if (satisfying_subsets.empty())
            std::cout << "impossible" << std::endl;
        else
            std::cout << min_length << std::endl;
    }

    return 0;
}
