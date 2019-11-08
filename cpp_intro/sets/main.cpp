#include <iostream>
#include <set>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    for (int t = 0; t < num_tests; t++) {
        int num_insertions;
        cin >> num_insertions;

        set<int> s;

        for (int i = 0; i < num_insertions; i++) {
            int insertion;
            cin >> insertion;

            int value;
            cin >> value;

            if (insertion == 0)
                s.insert(value);
            else
                s.erase(value);
        }

        if (s.empty())
            cout << "Empty" << endl;
        else {
            for (set<int>::iterator it = s.begin(); it != s.end(); it++)
                cout << *it << " ";
            cout << endl;
        }
    }
    return 0;
}
