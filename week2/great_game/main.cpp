#include <iostream>
#include <vector>
#include <limits>
#include <unordered_map>

#define HOLMES_RED 0
#define HOLMES_BLACK 1
#define MORIARTY_RED 2
#define MORIARTY_BLACK 3

int n;
std::vector<std::vector<int>> transitions;

std::unordered_map<std::pair<int, int>, int> memo[4];

int check_memo(int m, int r, int b) {
    std::pair<int, int> k(r, b);
    auto it = memo[m].find(k);
    if (it != memo[m].end())
        return it->second;
    else
        return -1;
}

void assign_memo(int m, int r, int b, int v) {
    std::pair<int, int> k(r, b);
    memo[m][k] = v;
}

void clear_memo() {
    for (int i = 0; i < 4; ++i) {
        memo[i].clear();
    }
}

int max_gain(int holmes, int moriarty, int red_location, int black_location) {
    // if red meeple at target, holmes wins
    if (red_location == n - 1)
        return 1;
    
    // if black meeple at target, moriarty wins
    if (black_location == n - 1)
        return 0;

    if ((moriarty + holmes) % 2 == 0) {
        int max = -1;
        if (holmes % 2 == 0) {
            int in_memo = check_memo(HOLMES_RED, red_location, black_location);
            if (in_memo != -1) return in_memo;
            // red meeple
            for (auto it = transitions[red_location].begin(); it != transitions[red_location].end(); ++it) {
                int gain = max_gain(holmes + 1, moriarty, *it, black_location);
                if (gain > max) {
                    max = gain;
                }
            }
            assign_memo(HOLMES_RED, red_location, black_location, max);
        } else {
            int in_memo = check_memo(HOLMES_BLACK, red_location, black_location);
            if (in_memo != -1) return in_memo;
            // black meeple
            for (auto it = transitions[black_location].begin(); it != transitions[black_location].end(); ++it) {
                int gain = max_gain(holmes + 1, moriarty, red_location, *it);
                if (gain > max) {
                    max = gain;
                }
            }
            assign_memo(HOLMES_BLACK, red_location, black_location, max);
       }
        return max;
    } else {
        int min = 2;        
        if (moriarty % 2 == 0) {
            int in_memo = check_memo(MORIARTY_BLACK, red_location, black_location);
            if (in_memo != -1) return in_memo;
             // black meeple
            for (auto it = transitions[black_location].begin(); it != transitions[black_location].end(); ++it) {
                int gain = max_gain(holmes, moriarty + 1, red_location, *it);
                if (gain < min) {
                    min = gain;
                }
            }
            assign_memo(MORIARTY_BLACK, red_location, black_location, min);
        } else {
            // red meeple
            for (auto it = transitions[red_location].begin(); it != transitions[red_location].end(); ++it) {
                int gain = max_gain(holmes, moriarty + 1, *it, black_location);
                if (gain < min) {
                    min = gain;
                }
            }
            assign_memo(MORIARTY_RED, red_location, black_location, min);
        }
        return min;
    }
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int m; std::cin >> n >> m;
        int r, b; std::cin >> r >> b;

        transitions.clear();

        clear_memo();

        for (int i = 0; i < n; ++i) {
            transitions.push_back(std::vector<int>());
        }

        for (int i = 0; i < m; ++i) {
            int u, v; std::cin >> u >> v;
            transitions[u - 1].push_back(v - 1);
        }

        int fuck = max_gain(0, 0, r - 1, b - 1);
        if (fuck == 0)
            std::cout << 1 << std::endl;
        else
            std::cout << 0 << std::endl;
    }
    return 0;
}
