#include <iostream>
#include <vector>
#include <algorithm>

std::vector<size_t> subsets;

int num_subsets(size_t together, size_t idx, int num_subs) {
    if (num_subs == 0) return 1;
    int result = 0;
    for (size_t i = idx; i < subsets.size(); i++) {
        if ((subsets[i] & together) == 0) {
            result += num_subsets(subsets[i] | together, i + 1, num_subs - 1);
        }
    }
    return result;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;

        int l[n];

        int total_perimeter = 0;
        for (size_t i = 0; i < n; i++) {
            std::cin >> l[i];
            total_perimeter += l[i];
        }

        subsets.clear();

        // O(n * 2^n)
        // this can be improved with split & list

        for (size_t subset = 0; subset < 1<<n; subset++) {
            int sum = 0;
            for (size_t i = 0; i < n; ++i)
                if (subset & 1<<i)
                    sum +=l[i];
            if (sum == total_perimeter / 4)
                subsets.push_back(subset);
        }

        // among these subsets, we need to find the number of
        // 4-tuple subsets that are disjoint





        std::cout << num_subsets(0, 0, 4) << std::endl;
    }

    return 0;
}
