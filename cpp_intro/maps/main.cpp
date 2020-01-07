#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    for (int t = 0; t < num_tests; t++) {
        int num_insertions;
        cin >> num_insertions;

        multimap<string, int> m;

        for (int i = 0; i < num_insertions; i++) {
            int v;
            string k;
            cin >> v >> k;
            if (v == 0) {
                m.erase(k);
            } else {
                m.insert(pair<string, int>(k, v));
            }
        }

        string k;
        cin >> k;

        pair<multimap<string, int>::iterator, multimap<string, int>::iterator> ret;
        ret = m.equal_range(k);

        vector<int> v;
        for (multimap<string, int>::iterator it = ret.first; it != ret.second; it++)
            v.push_back(it->second);

        if (v.empty())
            cout << "Empty" << endl;
        else {
            sort(v.begin(), v.end());
            for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
                cout << *it << " ";
            cout << endl;
        }
    }
    return 0;
}
