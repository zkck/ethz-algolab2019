#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    while (--num_tests >= 0) {
        int n;
        cin >> n;

        int matrix[n][n];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> matrix[i][j];
            }
        }

        int fucking_huge_sum = 0;

        for (int iteration = 0; iteration < n; iteration++) {

            int amount_summed = iteration + 1;

            int reduced_matrix[n - iteration][n];

            for (int j = 0; j < n; ++j) {
                for (int i = 0; i < n - iteration; ++i) {
                    int sum = 0;
                    for (int delta = 0; delta < amount_summed; ++delta) {
                        sum += matrix[i + delta][j];
                    }
                    reduced_matrix[i][j] = sum;
                }
            }



            for (int i = 0; i < n - iteration; ++i) {
                int sum = 0;
                int needs_even = 0;
                int needs_odd = 0;
                for (int j = 0; j < n; ++j) {
                    if (reduced_matrix[i][j] % 2 == 0) {
                        sum += ++needs_even;
                    } else {
                        sum += needs_odd;

                        // swap
                        int temp = needs_even;
                        needs_even = needs_odd;
                        needs_odd = temp;

                        needs_odd++;
                    }
                }
                fucking_huge_sum += sum;
            }
        }
        cout << fucking_huge_sum << endl;

    }
    return 0;
}
