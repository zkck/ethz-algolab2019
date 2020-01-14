#include <iostream>
#include <vector>

int num_sums(int brightness, int off[], int on[], int N, std::vector<size_t> &subsets)
{
    for (size_t ss = 0; ss < 1<<N; ss++) {
        long ss_sum;
        for (size_t i = 0; i < N; ++i) {
            if (1<<i & ss) ss += on[i];
            else           ss += off[i];
        }
        if (ss_sum = brightness) subsets.push_back(ss);
    }
    return subsets.size();
}

int disjoint_rec(std::vector<size_t> subsets[], int i, int M, size_t sofar, std::vector<size_t> disjoint) {
    if (i == M) {
        disjoint.push_back(sofar);
        return 1;
    }
    int count = 0;
    for (size_t ss : subsets[i]) {
        if ( !(ss & sofar)) {
            count += disjoint_rec(subsets, i + 1, M, sofar | ss, disjoint);
        }
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
        {
            std::cin >> b[i];
        }
        int on[N], off[N];
        for (size_t i = 0; i < N; i++)
        {
            std::cin >> on[i] >> off[i];
        }
        std::vector<size_t> subsets[M];
        for (size_t i = 0; i < M; i++) {
            num_sums(b[i], off, on, N, subsets[i]);
        }
        std::vector<size_t> disjoint_subsets;
        disjoint_rec(subsets, 0, M, 0, disjoint_subsets);

        // Find minimum
        size_t min_dss = 0;
        int min_count = N;
        for (size_t dss : disjoint_subsets)
        {
            int count = 0;
            for (size_t i = 0; i < M; i++)
                if (1<<i & dss) count++;
            if (count < min_count) {
                min_count = count;
                min_dss = dss;
            }
        }

        std::cout << min_count << std::endl;

    }

    return 0;
}
