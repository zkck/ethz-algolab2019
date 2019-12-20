#include <iostream>
#include <vector>


int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n, m, k; std::cin >> n >> m >> k;

        int partial_sums[n];

        int sum = 0;
        for (size_t i = 0; i < n; i++)
        {
            int v; std::cin >> v;
            sum += v;
            partial_sums[i] = sum;
        }

        std::vector<std::pair<size_t, size_t>> pairs;

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
            if (p_sum == k)
                pairs.push_back(std::make_pair(left, right));
        }

        // first groups of testsets
        //   - m == 2

        if (m == 1) {
            size_t max = 0;
            for (std::pair<int, int> p : pairs) {
                size_t num_defenders = right - left + 1;
                if (num_defenders > max)
                    max = num_defenders;
            }
        }

        
    }
    
    return 0;
}
