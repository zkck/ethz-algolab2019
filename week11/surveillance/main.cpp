#include <iostream>

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        // - n is number of intersections
        // - m is number of streets
        // - k is number of police stations
        // - l is number of photographs
        int n, m, k, l; std::cin >> n >> m >> k >> l;

        // police stations are at intersections
        int police_stations[k];
        for (size_t i = 0; i < k; i++)
            std::cin >> police_stations[i];

        // photographs are at intersections
        int photographs[l];
        for (size_t i = 0; i < l; i++)
            std::cin >> photographs[i];

        // streets connect intersections
        for (size_t i = 0; i < m; i++)
        {
            int x, y; std::cin >> x >> y;
        }




    }
    return 0;
}
