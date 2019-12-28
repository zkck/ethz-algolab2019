#include <iostream>
#include <vector>

// costs of reparation of each city
std::vector<int> costs;

// calculated minimum costs
std::vector<int> min_costs;

// children of each city, as per tree modeling of the problem
std::vector<std::vector<size_t>> children;

int selecting_cost(size_t i) {
    // check memo
    if (min_costs[i] > 0)
        return min_costs[i];
    int cost = costs[i];
    for (int c1 : children[i])
        for (int c2 : children[c1])
            cost += selecting_cost(c2);
    min_costs[i] = cost;
    return cost;

}

int main(int argc, char const *argv[])
{
    int num_tests;
    std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;

        children.clear();
        min_costs.clear();

        children.resize(n);
        min_costs.resize(n, -1);

        for (size_t stage = 0; stage < n - 1; stage++)
        {
            int i, j; std::cin >> i >> j;
            if (i < j)
                children[i].push_back(j);
            else
                children[j].push_back(i);
        }

        costs.clear();
        for (size_t i = 0; i < n; i++)
        {
            int c; std::cin >> c;
            costs.push_back(c);
        }
        

        // compare between selecting the 0-node or its children
        int selecting = selecting_cost(0);
        int not_selecting = 0;
        for (int c : children[0])
            not_selecting += selecting_cost(c);

        std::cout
            << (selecting > not_selecting ? not_selecting : selecting)
            << std::endl;
    }
    
    return 0;
}
