#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <random>
#include <limits>


std::vector<std::pair<int, int>> jedi;
int m;

std::vector<std::vector<size_t>> conflicts;

int interval_scheduling(int separation_point) {
    int a, b;

    std::vector<std::pair<int, int>> adapted_jedi;

    for (auto &j : jedi) {
        std::tie(a, b) = j;

        // two possible cases:
        //   - separation_point < a && a <= m
        //   - 0 < a && a < separation_point
        if (separation_point < a && a <= m) {
            // can be considered as is
            adapted_jedi.push_back(std::make_pair(a, b));
        } else if (0 < a && a < separation_point) {
            // has to be shifted
            adapted_jedi.push_back(std::make_pair(a + m, b + m));
        } else {
            throw std::runtime_error("invalid jedi");
        }
    }

    // for (auto &j : adapted_jedi) printf("  %d, %d\n", j.first, j.second);

    int count = 0;
    int expanded = true;
    int pointer = separation_point;
    while (expanded) {
        expanded = false;
        int earliest_ft = std::numeric_limits<int>::max();
        for (auto &j : adapted_jedi) {
            std::tie(a, b) = j;
            if (a > pointer) {
                expanded = true;
                earliest_ft = std::min(earliest_ft, b);
                // printf("    compatible, ft is %d\n", earliest_ft);
            }
        }
        if (expanded) {
            count++;
            // printf("  pointer %d -> %d\n", pointer, earliest_ft);
            pointer = earliest_ft;
        }
    }
    return count;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n >> m;

        jedi.clear();
        for (size_t i = 0; i < n; i++) {
            int a, b; std::cin >> a >> b;
            if (a <= b)
                jedi.push_back(std::make_pair(a, b));
            else
                jedi.push_back(std::make_pair(a, b + m));
        }

        // std::cout << "n=" << n << " m=" << m << std::endl;

        // // FIRST PHASE
        // //
        // // Build conflict map

        // int a1, b1, a2, b2;
        // for (size_t i = 0; i < n; i++) {
        //     std::tie(a1, b1) = jedi[i];
        //     std::vector<size_t> cs;
        //     for (size_t j = 0; j < n; j++) {
        //         if (j == i) continue;
        //         std::tie(a2, b2) = jedi[j];
        //         if (b1 > m) {
        //             if (a2 + m <= b1 || b2 >= a1) {
        //                 cs.push_back(j);
        //             }
        //         } else {
        //             if (b1 >= a2 && b2 >= a1 || b2 >= a1 + m) {
        //                 cs.push_back(j);
        //             }
        //         }
        //     }
        // }

        // SECOND PHASE
        //
        // Check if we can use an uncovered segment, in which case it resembles a simple
        // interval scheduling problem

        std::sort(jedi.begin(), jedi.end());
        int cover_start, cover_end; std::tie(cover_start, cover_end) = jedi[0];
        for (auto &j : jedi) {
            int a, b; std::tie(a, b) = j;
            if (a - cover_end > 1) break;
            cover_end = std::max(cover_end, b);
        }

        // std::cout << "  cover_start=" << cover_start << " cover_end=" << cover_end << std::endl;
        int separation_point = -1;
        if (cover_start + m - cover_end > 1) separation_point = (cover_end + 1) % m;
        // std::cout << "  separation=" << separation_point << std::endl;

        if (separation_point >= 0)
            std::cout << interval_scheduling(separation_point) << std::endl;
    }

    return 0;
}
