#include <iostream>
#include <vector>
#include <algorithm>

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;
        std::vector<int> lengths;
        int total_perimeter = 0;
        for (size_t i = 0; i < n; i++)
        {
            int l; std::cin >> l;
            lengths.push_back(l);
            total_perimeter += l;
        }

        std::sort(lengths.begin(), lengths.end());
        int side_length = total_perimeter / 4;

        for (size_t i = 0; i < n; i++)
        {
            int diff = side_length - lengths[i];
            for (size_t j = i + 1; j < n; j++)
            {
                if (lengths[j] == diff)
                {

                }
            }

        }




    }

    return 0;
}
