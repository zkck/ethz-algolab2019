#include <iostream>
#include <vector>
#include <map>
using namespace std;

vector<vector<int>> memo;

vector<int> coins;
int k;
int m;

int max_gain(int i, int left, int right) {

    // base case
    if (left == right) {
        return i % m == k ? coins[left] : 0;
    }

    // check memo
    if (memo[left][right] != -1) {
        return memo[left][right];
    }

    int res;

    // check if maximizing player's turn
    if (i % m == k) {
        int max_left  = coins[left]  + max_gain(i + 1, left + 1, right);
        int max_right = coins[right] + max_gain(i + 1, left, right - 1);
        res = max_left > max_right ? max_left : max_right;
    } else {
        int max_left  = max_gain(i + 1, left + 1, right);
        int max_right = max_gain(i + 1, left, right - 1);
        res = max_left > max_right ? max_right : max_left;
    }

    // memorize result
    memo[left][right] = res;

    return res;
}

int main(int argc, char const *argv[])
{
    int num_tests; cin >> num_tests;

    while (num_tests-- > 0) {
        int n;
        cin >> n >> m >> k;

        // reset memo between tests
        memo.clear();
        for (int i = 0; i < n; ++i) {
            vector<int> v(n, -1);
            memo.push_back(v);
        }

        // reset coin list between tests
        coins.clear();
        for (int i = 0; i < n; ++i) {
            int elem; cin >> elem;
            coins.push_back(elem);
        }

        cout << max_gain(0, 0, n - 1) << endl;
    }


    return 0;
}
