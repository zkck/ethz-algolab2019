#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    for (int t = 0; t < num_tests; t++) {
        int n;
        cin >> n;

        int needs_odd = 0;
        int needs_even = 0;

        int sum = 0;
        for (int i = 0; i < n; ++i) {
            int elem;
            cin >> elem;
            if (elem % 2 == 0) {
                sum += ++needs_even;
            } else {
                sum += needs_odd;
                
                // swap
                int temp = needs_even;
                needs_even = needs_odd;
                needs_odd  = temp;

                needs_odd++;
            }
        }

        cout << sum << endl;
    }
    return 0;
}
