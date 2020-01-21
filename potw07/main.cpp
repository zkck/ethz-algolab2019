#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <random>
#include <limits>


int interval_scheduling(std::vector<std::pair<int, int>> jedi, int m, int from, int to) {
    std::vector<std::pair<int, int>> adapted_jedi;
    if (from > to) { // a > b
        std::swap(from, to);
        // std::cout << "from=" << from << " to=" << to << std::endl;
        // easy case
        for (auto &j : jedi)
        {
            int a = j.first;
            int b = j.second;
            if (a <= b && from < a && b < to) adapted_jedi.push_back(j);
        }
    } else { // a <= b (from <= to)
        std::swap(from, to);
        // std::cout << "from=" << from << " to=" << to << std::endl;

        // harder case
        for (auto &j : jedi)
        {
            int a = j.first;
            int b = j.second;
            if (a <= b) {
                if (from < a) adapted_jedi.push_back(j);
                else if (b < to) adapted_jedi.push_back(std::make_pair(m + a, m + b));
            } else {
                if (from < a && b < to) adapted_jedi.push_back(std::make_pair(a, m + b));
            }
        }
    }
    std::sort(adapted_jedi.begin(), adapted_jedi.end());
    // for (auto &j : adapted_jedi) std::cout << "  " << j.first << " " << j.second << std::endl;
    int count = 0;
    int expanded = true;
    int pointer = from;
    while (true) {
        std::pair<int, int> lower_bound = std::make_pair(pointer + 1, 0);
        auto lower = std::lower_bound(adapted_jedi.begin(), adapted_jedi.end(), lower_bound);
        if (lower == adapted_jedi.end()) break;
        int earliest_ft = std::numeric_limits<int>::max();
        for (; lower != adapted_jedi.end(); lower++) {
            int ft = lower->second;
            if (ft < earliest_ft) {
                earliest_ft = ft;
            }
        }
        pointer = earliest_ft;
        count++;
    }
    return count;

}

int min_segment(std::vector<int> &cover_begins, std::vector<int> &cover_ends, int num_base) {
    auto beg_it = cover_begins.begin();
    auto end_it = cover_ends.begin();
    while (*beg_it == 1 && ++beg_it != cover_begins.end());
    while (*end_it == 1 && ++end_it != cover_ends.end());
    int jedi_count = num_base, min_count = num_base;
    int min_seg = 1;
    while (end_it != cover_ends.end() && beg_it != cover_begins.end()) {
        int seg = std::min(*end_it, *beg_it);
        if (*end_it < *beg_it) {
            jedi_count--;
            end_it++;
        } else if (*end_it > *beg_it) {
            jedi_count++;
            beg_it++;
        } else {
            beg_it++;
            end_it++;
        }
        if (jedi_count < min_count) {
            min_count = jedi_count;
            min_seg = seg;
        }
    }
    // if there are still jedi to be removed
    int still_removeable = 0;
    int final_seg = -1;
    while (end_it != cover_ends.end()) {
        still_removeable++;
        final_seg = *end_it++;
    }

    jedi_count -= still_removeable;
    if (jedi_count < min_count) min_seg = final_seg;

    return min_seg;

}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n, m; std::cin >> n >> m;

        std::vector<std::pair<int, int>> jedi;

        std::vector<int> cover_begins;
        std::vector<int> cover_ends;

        // number of jedi at the first segment
        int num_base = 0;

        for (size_t i = 0; i < n; i++) {
            int a, b; std::cin >> a >> b;
            if (a > b || a == 1) num_base++;
            jedi.push_back(std::make_pair(a, b));
            cover_begins.push_back(a);
            cover_ends.push_back((b + 1) % m);
        }

        // IDEA 1
        //
        // To find the segment with the least amount of clones, order the start and end
        // positions of the jedi. Then with two pointers, keep track of when a jedi's cover
        // ends and when it starts, and find the position where the number is minimal.

        std::sort(cover_begins.begin(), cover_begins.end());
        std::sort(cover_ends.begin(), cover_ends.end());
        int min_seg = min_segment(cover_begins, cover_ends, num_base);

        // std::cout << min_seg << std::endl;

        // get all jedi on which to do the interval scheduling
        std::vector<std::pair<int, int>> interval_scheduling_jedi;
        for (auto &j : jedi) {
            int a = j.first;
            int b = j.second;
            if (a <= b) {
                if (a <= min_seg && min_seg <= b)
                    interval_scheduling_jedi.push_back(j);
            } else {
                if (a <= min_seg || min_seg <= b)
                    interval_scheduling_jedi.push_back(j);
            }
        }

        // assert(interval_scheduling_jedi.size() == 0);

        int max_jedi = interval_scheduling(jedi, m, min_seg, min_seg);
        for (auto &is_j : interval_scheduling_jedi) {
            // std::cout << is_j.first << " " << is_j.second << std::endl;
            max_jedi = std::max(max_jedi, 1 + interval_scheduling(jedi, m, is_j.first, is_j.second));
        }

        std::cout << max_jedi << std::endl;



    }

    return 0;
}
