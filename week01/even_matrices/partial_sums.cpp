#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    while (num_tests-- > 0) {
        int n;
        cin >> n;

        int elements[n][n];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> elements[i][j];
            }
        }

        cout << "Partial sums" << endl;

        int partial_sums[n][n];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                int top  = (i - 1 < 0) ? 0 : partial_sums[i - 1][j];
                int left = (j - 1 < 0) ? 0 : partial_sums[i][j - 1];

                int topleft = (i - 1 < 0 || j - 1 < 0)
                    ? 0
                    : partial_sums[i - 1][j - 1];

                partial_sums[i][j] = (top + left) - topleft + elements[i][j];
                cout << partial_sums[i][j] << " ";
            }
            cout << endl;
        }

    }
    return 0;
}
