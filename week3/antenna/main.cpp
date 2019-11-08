#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

double ceil_to_double(const K::FT& x){
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a - 1 >= x) a -= 1;
    return a;
}

int main(int argc, char const *argv[])
{
    while (true) {
        long n; std::cin >> n;
        if (n == 0) break;

        std::vector<K::Point_2> citizens;

        for (int i = 0; i < n; ++i) {
            long x, y; std::cin >> x >> y;
            citizens.push_back(K::Point_2(x, y));
        }

        Min_circle mc(citizens.begin(), citizens.end(), true);
        Traits::Circle c = mc.circle();

        long sq = ceil_to_double(CGAL::sqrt(c.squared_radius()));
        std::cout << sq << std::endl;
    }
    return 0;
}
