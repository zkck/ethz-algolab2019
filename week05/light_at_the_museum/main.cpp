#include <iostream>
#include <vector>

void binary_print(size_t i, int N) {
    for (size_t idx = 0; idx < N; ++idx) {
        if (i & 1<<idx) std::cout << '1';
        else            std::cout << '0';
    }
    std::cout << std::endl;
}

int num_sums(int brightness,
    int off[],
    int on[],
    int i,
    int N,
    int subset,
    int mask,
    std::vector<int> &subsets,
    std::vector<int> &masks)
{
    if (i == N) {
        if (brightness == 0) {
            subsets.push_back(subset);
            masks.push_back(mask);
            return 1;
        } else {
            return 0;
        }
    }
    int result = 0;
    if (on[i] == 0 && off[i] == 0)
        result += num_sums(brightness, off, on, i + 1, N, subset, mask, subsets, masks);
    else {
        result += num_sums(brightness - off[i],
            off,
            on,
            i + 1,
            N,
            subset,         // does not change since we are adding an off (0) switch
            mask | (1<<i),  // changes because we are "setting" this switch
            subsets,
            masks);
        result += num_sums(brightness - on[i],
            off,
            on,
            i + 1,
            N,
            subset | (1<<i),    // changes because we have the switch on
            mask | (1<<i),      // changes because we are "setting" this switch
            subsets,
            masks);
    }
    return result;
}

int num_compatible(std::vector<int> subsets[], std::vector<int> masks[], int i, int M, int mask, int subset) {
    // TODO
    return 0;
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
        int on[M][N], off[M][N];
        for (size_t i = 0; i < N; i++)
            for (size_t j = 0; j < M; j++)
                std::cin >> on[j][i] >> off[j][i];

        std::vector<int> subsets[M];
        std::vector<int> masks[M];
        for (size_t i = 0; i < M; i++) {
            num_sums(b[i], off[i], on[i], 0, N, 0, 0, subsets[i], masks[i]);
        }



    }

    return 0;
}
