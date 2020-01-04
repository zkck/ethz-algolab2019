#include <iostream>
#include <vector>
#include <queue>

// memoization
std::vector<int> memo;

// as per tree modeling of the problem
std::vector<std::vector<int>> children;

// costs of rebuilding a city
std::vector<int> costs;

int force_cover(int i) {
    if (memo[i] >= 0) return memo[i];

    // option 1: select the node and cover subchildren

    int result = costs[i];
    for (int c1 : children[i])
        for (int c2 : children[c1])
            result += force_cover(c2);

    // option 2: cover using the children

    for (int c : children[i])
    {
        int cc = costs[c];

        // add costs of covering subchildren
        for (int c1 : children[c])
            for (int c2 : children[c1])
                cc += force_cover(c2);
    

        // and costs of covering other children
        for (int other : children[i])
            if (c != other)
                cc += force_cover(other);

        if (cc < result) result = cc;
    }

    memo[i] = result;
    return result;
}

int main(int argc, char const *argv[])
{
    int num_tests;
    std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        // get number of cities
        int n; std::cin >> n;

        children.clear();
        children.resize(n);

        for (size_t stage = 0; stage < n - 1; stage++)
        {
            int i, j; std::cin >> i >> j;
            children[i].push_back(j);
        }

        costs.clear();

        for (size_t i = 0; i < n; i++)
        {
            int c; std::cin >> c;
            costs.push_back(c);
        }

        memo.clear();
        memo.resize(n, -1);

        std::cout << force_cover(0) << std::endl;
        
    }
    
    return 0;
}
