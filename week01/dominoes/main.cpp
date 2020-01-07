#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    while (--num_tests >= 0) {
        int n;
        cin >> n;

        int heights[n];
        for (int i = 0; i < n; ++i) cin >> heights[i];

        int num_fallen = 0;

        int topple_index = 0;
        int topple_dist = heights[0];

        while (topple_index < n && topple_index < topple_dist) {
            num_fallen++;
            int h = heights[topple_index];
            if (h + topple_index > topple_dist) {
                topple_dist = h + topple_index;
            }
            topple_index++;
        }

        cout << num_fallen << endl;
    }
    return 0;
}
