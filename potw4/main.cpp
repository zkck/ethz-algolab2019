#include <iostream>
#include <vector>
#include <queue>
#include <limits>

// memoization
std::vector<int> uncovered_memo, covered_memo;

// as per tree modeling of the problem
std::vector<std::vector<int>> children;

// costs of rebuilding a city
std::vector<int> costs;

int uncovered_cover(int i);
int covered_cover(int i);
int selected_cover(int i);

int uncovered_cover(int i) {
    if (uncovered_memo[i] >= 0) return uncovered_memo[i];
    int selection_cost = selected_cover(i);
    int exclusion_cost = std::numeric_limits<int>::max();
    for (int c1 : children[i]) {
        int child_cost = selected_cover(c1);
        for (int c2 : children[i])
            if (c1 != c2)
                child_cost += uncovered_cover(c2);
        if (child_cost < exclusion_cost) exclusion_cost = child_cost;
    }
    int result = std::min(selection_cost, exclusion_cost);
    uncovered_memo[i] = result;
    return result;
}

int covered_cover(int i) {
    if (covered_memo[i] >= 0) return covered_memo[i];
    int selection_cost = selected_cover(i);
    int exclusion_cost = 0;
    for (int c : children[i])
        exclusion_cost += uncovered_cover(c);
    int result = std::min(selection_cost, exclusion_cost);
    covered_memo[i] = result;
    return result;
}

int selected_cover(int i) {
    int result = costs[i];
    for (int c1 : children[i])
        result += covered_cover(c1);
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

        uncovered_memo.clear();
        uncovered_memo.resize(n, -1);
        covered_memo.clear();
        covered_memo.resize(n, -1);

        std::cout << uncovered_cover(0) << std::endl;
        
    }
    
    return 0;
}
