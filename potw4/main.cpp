#include <iostream>
#include <vector>
#include <queue>
#include <limits>

// memoization
std::vector<int> uncovered_memo, covered_memo, selected_memo;

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

    // 
    std::vector<int> children_uncovered_costs;
    int children_uncovered_costs_total = 0;
    for (int c1 : children[i]) {
        int cost = uncovered_cover(c1);
        children_uncovered_costs.push_back(cost);
        children_uncovered_costs_total += cost;
    }

    //
    int exclusion_cost = std::numeric_limits<int>::max();
    for (int c_idx = 0; c_idx < children[i].size(); c_idx++) {
        int cost = children_uncovered_costs_total 
            - children_uncovered_costs[c_idx] 
            + selected_cover(children[i][c_idx]);
        if (cost < exclusion_cost) exclusion_cost = cost;
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
    if (selected_memo[i] >= 0) return selected_memo[i];
    int result = costs[i];
    for (int c1 : children[i])
        result += covered_cover(c1);
    selected_memo[i] = result;
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
        selected_memo.clear();
        selected_memo.resize(n, -1);

        // hint implementation: do a DFS to prep the memo


        std::cout << uncovered_cover(0) << std::endl;
        
    }
    
    return 0;
}
