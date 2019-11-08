#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    while (num_tests-- > 0) {
        int n;
        cin >> n;

        int sum = 0;
        while (n-- > 0) {
            int elem; cin >> elem;
            sum += elem;
        }

        cout << sum << endl;
    }
    return 0;
}
