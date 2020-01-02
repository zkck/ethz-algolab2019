#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n, k; std::cin >> n >> k;

        // beverage is a (cost, volume) pair
        std::vector<std::pair<int, int>> beverages;
        beverages.reserve(n);
        
        for (size_t i = 0; i < n; i++)
        {
            int c, v; std::cin >> c >> v;
            beverages.push_back(std::make_pair(c, v));
        }
        
        // find best drink for remaining liters
        int remaining = k;
        int cost = 0;
        while (remaining > 0) {
            int bd = -1;
            double bd_ratio = -std::numeric_limits<double>::infinity();
            for (size_t i = 0; i < n; i++)
            {
                std::pair<int, int> bev = beverages[i];
                double ratio = bev.second / (double) bev.first;
                if (ratio > bd_ratio) {
                    bd = i;
                    bd_ratio = ratio;
                }
            }
            remaining -= beverages[bd].second;
            cost += beverages[bd].first;
        }

        std::cout << cost << std::endl;
    }
    
    return 0;
}
