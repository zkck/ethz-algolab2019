#include <iostream>
#include <string>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    for (int t = 0; t < num_tests; t++) {
        string str1, str2;
        cin >> str1 >> str2;

        cout << str1.length() << " " << str2.length() << endl;

        cout << str1 << str2 << endl;

        for (int i = 0; i < str1.length(); i++) {
            if (i == 0) {
                cout << str2[str2.length() - 1];
            } else {
                cout << str1[str1.length() - 1 - i];
            }
        }

        cout << " ";

        for (int i = 0; i < str2.length(); i++) {
            if (i == 0) {
                cout << str1[str1.length() - 1];
            } else {
                cout << str2[str2.length() - 1 - i];
            }
        }

        cout << endl;
    }
    return 0;
}
