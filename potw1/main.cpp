#include <iostream>
#include <limits>

int abs(int n) {
    return n < 0 ? n * -1 : n;
}

int main(int argc, char const *argv[])
{
    int num_tests;
    std::cin >> num_tests;

    for (int t = 0; t < num_tests; ++t) {
        int n, k;
        std::cin >> n >> k;

        int array[n];

        int sum = 0;
        for (int i = 0; i < n; ++i) {
            int e;
            std::cin >> e;
            sum += e;
            array[i] = sum;
        }

        int best_dist = std::numeric_limits<int>::max();
        int best_i = 0;
        int best_j = -1;

        int j = -1;
        for (int i = 0; i < n; ++i) {

            int curr_val = array[i] - (j < 0 ? 0 : array[j]);
            // std::cout << "curr_val=" << curr_val;
            int dist_to_k = abs(k - curr_val);

            // std::cout << "dist to k set at " << dist_to_k << " for i=" << i << " j=" << j << std::endl;

            // find best j for that i
            while (abs(k - (array[i] - array[j + 1])) < dist_to_k && j + 1 < i) {
                j++;
                dist_to_k = abs(k - (array[i] - array[j]));
                // std::cout << "better dist to k for i=" << i << " and j=" << j << " at dist=" << dist_to_k << " " << std::endl;
            }

            if (dist_to_k < best_dist) {
                // std::cout << "updated to i=" << i << " and j=" << j << std::endl;
                best_i = i;
                best_j = j;
                best_dist = dist_to_k;
            }        
        }

        std::cout << (best_j + 1) << " " << (best_i) << std::endl;

    }
    return 0;
}
