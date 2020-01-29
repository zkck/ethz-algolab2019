#include <iostream>
#include <vector>
#include <algorithm>
#include <map>


int main(int argc, char const *argv[])
{
    std::ios_base::sync_with_stdio(false);
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;

        int m[n];
        for (size_t i = 0; i < n; i++) {
            std::cin >> m[i];
        }

        // IDEA
        //
        // Keep a sorted list of positions

        std::multimap<int, size_t> positions;

        std::vector<int> p[n];
        for (size_t i = 0; i < n; i++) {
            p[i].resize(m[i]);
            for (size_t j = 0; j < m[i]; j++) {
                std::cin >> p[i][j];
                positions.insert(std::make_pair(p[i][j], i));
            }
        }

        std::vector<int> occurrences(n, 0);

        int min = std::numeric_limits<int>::max();

        auto l = positions.begin();
        auto r = positions.begin();
        for (int num_distinct = 0; r != positions.end(); ++r) {

            // new word
            if (occurrences[r->second]++ == 0)
                num_distinct++;

            // increasing section
            while (num_distinct < n) {
                if (occurrences[(++r)->second]++ == 0)
                    num_distinct++;
            }

            // decreasing section
            while (occurrences[l->second] > 1) {
                occurrences[(l++)->second]--;
            }

            if (num_distinct == n) {
                min = std::min(min, r->first - l->first + 1);
            }
        } while (r != positions.end());

        std::cout << min << std::endl;
    }

    return 0;
}
