#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

std::vector<std::pair<int, int>> wizards;



int max_num_greedy_complex(int n) {

    // by earliest finish time

    double pos = -std::numeric_limits<double>::infinity();
    int num = 0;

    std::sort(wizards.begin(), wizards.end(), [](std::pair<int, int> &v1, std::pair<int, int> &v2) {
        return v1.first < v2.first;
    });

    int i = -1;
    while (true) {

        int best_j = -1;
        double best_finishing_time = std::numeric_limits<double>::infinity();

        for (int j = i + 1; j < n; ++j) {
            if (pos > wizards[j].first)
                continue;
            if (wizards[j].first >= best_finishing_time)
                break;
            double finishing_time = pos > wizards[j].first - wizards[j].second
                ? pos + wizards[j].second
                : wizards[j].first;
            if (finishing_time < best_finishing_time) {
                best_j = j;
                best_finishing_time = finishing_time;
            }
        }
        if (best_j < 0)
            break;
        else {
            i = best_j;
            pos = best_finishing_time;
            num++;
        }
    }

    return num;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int n; std::cin >> n;

        wizards.clear();

        for (int i = 0; i < n; ++i) {
            int b; std::cin >> b;
            int r; std::cin >> r;
            wizards.push_back(std::pair<int, int>(r, b));
        }
        
        std::cout << max_num_greedy_complex(n) << std::endl;
    }
}