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
        if (separation_point < a && a <= m) {
            adapted_jedi.push_back(std::make_pair(a, b));
        } else if (0 < a && a < separation_point) {
            adapted_jedi.push_back(std::make_pair(a + m, b + m));
        } else {
            throw std::runtime_error("invalid jedi");
        }
    }

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
            }
        }
        if (expanded) {
            count++;
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

        std::sort(jedi.begin(), jedi.end());
        int cover_start, cover_end; std::tie(cover_start, cover_end) = jedi[0];
        for (auto &j : jedi) {
            int a, b; std::tie(a, b) = j;
            if (a - cover_end > 1) break;
            cover_end = std::max(cover_end, b);
        }

        int separation_point = -1;
        if (cover_start + m - cover_end > 1) separation_point = (cover_end + 1) % m;

        if (separation_point >= 0)
            std::cout << interval_scheduling(separation_point) << std::endl;
        else;
            // TODO
    }

    return 0;
}
