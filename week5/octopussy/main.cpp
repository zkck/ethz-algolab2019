#include <iostream>
#include <limits>
#include <vector>

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;
        int t[n];
        for (size_t i = 0; i < n; i++) {
            std::cin >> t[i];
            if (i > 0) {
                // adapt timing information to the parent
                size_t parent_i = (i - 1) / 2;
                t[i] = std::min(t[i], t[parent_i] - 1);
            }
        }

        int defusable[n];
        for (size_t i = 0; i < n; i++) {
            defusable[i] = i >= (n / 2);
        }

        int num_defused = 0;
        std::vector<int> defused(n, false);

        int boom = false, time = 1;
        while (!boom && !(num_defused == n)) {
            int choice = -1;
            int choice_diff = std::numeric_limits<int>::max();
            for (int i = 0; i < n; i++) {
                if (defusable[i]) {
                    int diff = t[i] - time;
                    if (diff < choice_diff) {
                        choice = i;
                        choice_diff = diff;
                    } else if (diff == choice_diff && diff == 0) {
                        boom = true;
                    }
                }
            }

            // defuse the choice
            defusable[choice] = false;
            defused[choice] = true;
            num_defused++;

            // set the parent to defusable
            int sibling = (choice % 2 == 0)
                ? choice - 1
                : choice + 1;
            int parent = (choice - 1) / 2;
            if (choice > 0 && defused[sibling]) defusable[parent] = true;

            time++;
        }

        if (boom) std::cout << "no" << std::endl;
        else      std::cout << "yes" << std::endl;
        
        
    }
    
    return 0;
}
