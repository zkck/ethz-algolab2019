#include <iostream>
#include <limits>
#include <vector>
#include <map>

std::vector<std::pair<int, int>> jedi;

int m;

int num_jedi_reverse(int start, int end) {
    // assuming end < start
    int count = 0;

    int pointer = start;

    int jedi_available = true;

    int passed = false;

    while (jedi_available) {
        jedi_available = false;
        int earliest_finish_time = std::numeric_limits<int>::max();

        // find jedi with earliest finish time
        for (auto j : jedi) {
            int a, b; std::tie(a, b) = j;
            if (passed)
                if (a > b) {
                    // covers segment a to b
                    if (a > pointer && b < end) {
                        // std::cout << "    new starting time with a=" << a << " b=" << b << std::endl;
                        jedi_available = true;
                        earliest_finish_time = std::min(earliest_finish_time, b);
                    }
                } else {
                    // a <= b
                    if (a > pointer) {
                        // std::cout << "    that new starting time with a=" << a << " b=" << b << std::endl;
                        jedi_available = true;
                        earliest_finish_time = std::min(earliest_finish_time, b - m);
                    } else if (b < end) {
                        // std::cout << "    this new starting time with a=" << a << " b=" << b << std::endl;
                        jedi_available = true;
                        earliest_finish_time = std::min(earliest_finish_time, b);
                    }
            }
            else
            if (a <= b) {
                if (a > pointer && b < end) {
                    jedi_available = true;
                    earliest_finish_time = std::min(earliest_finish_time, b);
                }
            }
        }

        if (jedi_available) {
            int new_pointer = earliest_finish_time <= 0
                ? earliest_finish_time + m
                : earliest_finish_time;
            // std::cout << "    pointer " << pointer << " -> " << new_pointer << std::endl;
            if (new_pointer < pointer) passed = true;
            pointer = new_pointer;
            count++;
        }

    }

    return count;
}

int num_jedi(int start, int end) {
    // assuming start < end
    int count = 0;

    int pointer = start;

    int jedi_available = true;

    while (jedi_available) {
        jedi_available = false;
        int earliest_finish_time = std::numeric_limits<int>::max();

        // find jedi with earliest finish time
        for (auto j : jedi) {
            int a, b; std::tie(a, b) = j;
            if (a <= b) {
                // covers segment a to b
                if (a > pointer && b < end) {
                    jedi_available = true;
                    earliest_finish_time = std::min(earliest_finish_time, b);
                }
            } else {
                // this overlaps anyway because of the precondition
            }
        }

        if (jedi_available) {
            pointer = earliest_finish_time;
            count++;
        }
    }

    // std::cout << "  num_jedi(" << start << ", " << end << ") = " << count << std::endl;

    return count;

}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int n; std::cin >> n >> m;

        // std::cout << "n=" << n << " m=" << m << std::endl;

        jedi.clear();
        for (size_t i = 0; i < n; i++)
        {
            int a, b; std::cin >> a >> b;
            jedi.push_back(std::make_pair(a, b));
        }

        //
        int best_m = -1;
        int best_count = std::numeric_limits<int>::max();

        std::vector<size_t> starting_jedi;


        for (size_t i = 1; i <= m; i++)
        {
            std::vector<size_t> maybe_starting_jedi;
            int count = 0;
            for (size_t idx = 0; idx < n; idx++) {
                int a, b; std::tie(a, b) = jedi[idx];
                if (a <= b) {
                    if (a <= i && i <= b) {
                        maybe_starting_jedi.push_back(idx);
                        count++;
                    }
                } else {
                    if (a <= i || i <= b) {
                        maybe_starting_jedi.push_back(idx);
                    }
                }
            }
            if (count < best_count) {
                best_count = count;
                best_m = i;
                starting_jedi = maybe_starting_jedi;
            }
        }

        int highest_jedi_count = -1;

        for (size_t s_idx : starting_jedi) {
            int a, b; std::tie(a, b) = jedi[s_idx];
            // std::cout << "  " << a << " " << b << std::endl;
            if (a > b)
                highest_jedi_count = std::max(highest_jedi_count, 1 + num_jedi(b, a));
            else
                highest_jedi_count = std::max(highest_jedi_count, 1 + num_jedi_reverse(b, a));
        }

        std::cout << std::max(highest_jedi_count, num_jedi_reverse(best_m - 1, best_m)) << std::endl;

    }
    return 0;
}
