#include <iostream>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    for (int i = 0; i < num_tests; i++) {
        int arr_length;
        cin >> arr_length;
        int sum = 0;
        for (int idx = 0; idx < arr_length; idx++) {
            int newval;
            cin >> newval;
            sum += newval;
        }
        cout << sum << endl;
    }
    return 0;
}
