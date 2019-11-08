#include <iostream>
#include <string>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    for (int t = 0; t < num_tests; t++) {
        int i;
        cin >> i;
        long l;
        cin >> l;
        string s;
        cin >> s;
        float f;
        cin >> f;
        cout << i << " ";
        cout << l << " ";
        cout << s << " ";
        printf("%.2f\n", f);
    }
    return 0;
}
