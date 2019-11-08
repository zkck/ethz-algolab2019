#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

bool pos_ok(int l, int r) {
    return (r - l) <= 200; 
}

int main(int argc, char const *argv[])
{
    int t; std::cin >> t;
    while (t-- > 0) {
        int n; std::cin >> n;

        // read that shit
        std::vector<int> parasols;
        for (int i = 0; i < n; ++i) {
            int elem; std::cin >> elem;
            parasols.push_back(elem);
        }

        // sort that shit
        std::sort(parasols.begin(), parasols.end());

        // algorithm that shit

        std::vector<int> best_beaches;
        int best_dist = std::numeric_limits<int>::max();
        int best_num = -1;

        for (int l = 0, r = 0; r < n; ++r) {
            while (parasols[r] - parasols[l] > 200) l++;

            int num_covered = r - l + 1;
            double pos = (parasols[r] + parasols[l]) / 2.0;

            int dist_left  = std::floor(pos) - parasols[l];
            int dist_right = parasols[r] - std::floor(pos);

            int dist_max = dist_left > dist_right ? dist_left : dist_right;

            if (num_covered > best_num) {
                best_beaches.clear();
                best_num = num_covered;
                best_dist = dist_max;
                best_beaches.push_back(std::floor(pos));
                if (std::floor(pos) != std::ceil(pos))
                    best_beaches.push_back(std::ceil(pos));
            } else if (num_covered == best_num) {
                if (dist_max < best_dist) {
                    best_beaches.clear();
                    best_dist = dist_max;
                    best_beaches.push_back(std::floor(pos));
                    if (std::floor(pos) != std::ceil(pos))
                        best_beaches.push_back(std::ceil(pos));
                } else if (dist_max == best_dist) {
                    best_beaches.push_back(std::floor(pos));
                    if (std::floor(pos) != std::ceil(pos))
                        best_beaches.push_back(std::ceil(pos));
                }
            }
        }

        std::cout << best_num << " " << best_dist << std::endl;

        for (auto it = best_beaches.begin(); it != best_beaches.end(); ++it) {
            std::cout << *it << (it + 1 == best_beaches.end() ? "" : " ");
        }
        std::cout << std::endl;
    }
    return 0;
}
