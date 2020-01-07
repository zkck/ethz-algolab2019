#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>


void algo()
{
    int n, m; std::cin >> n >> m;

    std::vector<int> strengths;
    std::vector<int> weights;

    for (int i = 0; i < n; i++)
    {
        int s; std::cin >> s;
        strengths.push_back(s);
    }

    for (int i = 0; i < m; i++)
    {
        int w; std::cin >> w;
        weights.push_back(w);
    }

    std::sort(strengths.begin(), strengths.end());
    std::sort(weights.begin(), weights.end());

    std::reverse(strengths.begin(), strengths.end());
    std::reverse(weights.begin(), weights.end());

    if (weights[0] > strengths[0]) {
        std::cout << "impossible" << std::endl;
        return;
    }

    int highest_pile = 0;
    int free_spots = 0;

    int box = 0;

    for (int assignee = 0; assignee < n && box < m; assignee++)
    {
        int box_count = 0;

        int next = assignee + 1;
        if (next < n)
            // take boxes that are too heavy for next assignee
            while (box < m && weights[box] > strengths[next]) {
                box++;
                box_count++;
            }
        else
        {
            // if last assignee, takes remaining boxes
            box_count = m - box;
            box = m;
        }

        // std::cout << "I am assignee " << assignee << " and I am taking " << box_count << std::endl;

        // three cases
        //   - pile below highest pile
        //   - pile is above highest pile but we can allocate free spots
        //   - pile is above highest pile and we need new free spots

        if (box_count <= highest_pile) {
            // std::cout << "  placing on pile..." << std::endl;
            free_spots += highest_pile - box_count;
        } else {
            // we have to distribute the overflow
            int overflow = box_count - highest_pile;
            if (overflow <= free_spots) {
                // std::cout << "  redistributing free..." << std::endl;
                // distribute overflow in free spots
                free_spots -= overflow;
            } else {
                // std::cout << "  making free..." << std::endl;
                // use up all free spots
                int remaining = overflow - free_spots;
                // distribute remaining across strongest
                int num_rows = std::ceil( ((double) remaining) / (assignee + 1));
                int top_row  = remaining % (assignee + 1);
                free_spots = (assignee + 1) - top_row;
                highest_pile += num_rows;
            }
        }


    }

    std::cout << ((highest_pile - 1) * 3 + 2) << std::endl;
}


int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) algo();
    return 0;
}
