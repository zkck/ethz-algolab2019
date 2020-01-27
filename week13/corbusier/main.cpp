#include <iostream>
#include <vector>

#define UNKNOWN     -1

// n : number of disks, i & k : Modulor params
int n, i, k;

// disk heights
std::vector<int> heights;

// memo
std::vector<std::vector<int>> memo;

int is_possible(int d, int h, int used) {
    // end condition
    if (used && h == i)    return true;
    if (d == n)            return false;

    if (used && memo[d][h] >= 0) return memo[d][h];

    int result = is_possible(d + 1, h, used) || is_possible(d + 1, (h + heights[d]) % k, true);

    if (used) memo[d][h] = result;

    return result;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        std::cin >> n >> i >> k;

        heights.clear();
        for (size_t i = 0; i < n; i++)
        {
            int h; std::cin >> h;
            heights.push_back(h);
        }

        memo.clear();
        memo.resize(n);
        for (auto &vect : memo) vect.resize(k, UNKNOWN);

        if (is_possible(0, 0, false)) {
            std::cout << "yes" << std::endl;
        } else {
            std::cout << "no" << std::endl;
        }
    }

    return 0;
}
