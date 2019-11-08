#include <iostream>
#include <vector>

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int zones, jobs; std::cin >> zones >> jobs;

        int prices[zones];
        for (int i = 0; i < zones; ++i)
            std::cin >> prices[i];

        int rewards[jobs];
        for (int i = 0; i < jobs; ++i)
            std::cin >> rewards[i];

        // std::vector<int> required_tickets[jobs];
        // for (int i = 0; i < jobs; ++i) {
        //     int n; std::cin >> n;
        //     for (int j = 0; j < n; ++j) {
        //         int z; std::cin >> z;
        //         required_tickets[i].push_back(z);
        //     }
        // }

        // testset 1

        int profits[zones];
        for (int i = 0; i < zones; ++i) {
            profits[i] = -prices[i];
        }

        int profit = 0;

        for (int i = 0; i < jobs; ++i) {
            int n; std::cin >> n; // assuming n is 1
            if (n == 0) {
                profit += rewards[i];
            }
            for (int j = 0; j < n; ++j) {
                int z; std::cin >> z;
                profits[z] += rewards[i];
            }
        }

        for (int j = 0; j < zones; ++j) {
            if (profits[j] > 0)
                profit += profits[j];
        }

        std::cout << profit << std::endl;
    }
    return 0;
}
