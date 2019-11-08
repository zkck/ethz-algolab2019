#include <iostream>
#include <vector>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    for (int t = 0; t < num_tests; ++t) {
        size_t size;
        cin >> size;

        vector<int> v;
        for (int i = 0; i < size; ++i) {
            int a;
            cin >> a;
            v.push_back(a);
        }

        int erase_idx;
        cin >> erase_idx;
        v.erase(v.begin() + erase_idx);

        int erase_start, erase_end;
        cin >> erase_start >> erase_end;
        v.erase(v.begin() + erase_start, v.begin() + erase_end + 1);

        for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
            cout << *it << " ";
        cout << "\n";
    }
    return 0;
}
