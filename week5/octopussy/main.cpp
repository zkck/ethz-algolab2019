#include <iostream>
#include <limits>
#include <vector>
#include <map>

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

        std::multimap<int, int> defusables;
        for (int i = 0; i < n; i++) {
            if (i >= n / 2) defusables.insert(std::make_pair(t[i], i));
        }

        int num_defused = 0;
        std::vector<int> defused(n, false);

        int boom = false, time = 1;
        while (!boom && !defusables.empty()) {
            auto choice = defusables.begin();

            int explosion_time = choice->first;
            int index = choice->second;
            if (explosion_time < time) {
                boom = true;
            }

            // defuse the choice
            defused[index] = true;
            defusables.erase(choice);

            // add the parent to defusables
            int sibling = (index % 2 == 0)
                ? index - 1
                : index + 1;
            int parent = (index - 1) / 2;
            if (index > 0 && defused[sibling]) {
                defusables.insert(std::make_pair(t[parent], parent));
            }

            time++;
        }

        if (boom) std::cout << "no" << std::endl;
        else      std::cout << "yes" << std::endl;
        
        
    }
    
    return 0;
}
