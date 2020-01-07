#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <vector>

typedef  CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K;
typedef  CGAL::Min_circle_2_traits_2<K>  Traits;
typedef  CGAL::Min_circle_2<Traits>      Min_circle;

K::FT squared_distance_without_point(Min_circle& mc, const K::Point_2& p) {
    std::vector<K::Point_2> points;
    for (auto it = mc.points_begin(); it != mc.points_end(); ++it) {
        if (*it != p) points.push_back(*it);
    }
    Min_circle mc_mod(points.begin(), points.end());
    return mc_mod.circle().squared_radius();
}

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
        if (mc.number_of_support_points() > 3) {
            std::cout << (long) ceil_to_double(CGAL::sqrt(mc.circle().squared_radius())) << std::endl;
            continue;
        }

        bool set = false;
        K::FT best;

        for (auto it = mc.support_points_begin(); it != mc.support_points_end(); ++it) {
            K::FT sq_dist = squared_distance_without_point(mc, *it);
            if (!set || sq_dist < best) {
                best = sq_dist;
                set = true;
            }
        }

        std::cout << (long) ceil_to_double(CGAL::sqrt(best)) << std::endl;
    }
    return 0;
}
