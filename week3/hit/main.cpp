#include <iostream>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/intersections.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2    Point_2;
typedef K::Ray_2      Ray_2;
typedef K::Segment_2  Segment_2;

int main(int argc, char const *argv[])
{
    while (true) {
        int n; std::cin >> n;
        if (n == 0) break;

        long x, y, a, b;
        std::cin >> x >> y >> a >> b;

        Point_2 src(x, y);
        Point_2 dst(a, b);

        Ray_2 ray(src, dst);

        bool intersection = false;

        for (int i = 0; i < n; ++i) {
            long r, s, t, u;
            std::cin >> r >> s >> t >> u;

            if (!intersection) {
                    
                // build segment
                Point_2 p1(r, s);
                Point_2 p2(t, u);

                Segment_2 segment(p1, p2);

                if (CGAL::do_intersect(segment, ray))
                    intersection = true;

            }
        }

        std::cout << (intersection ? "yes" : "no") << std::endl;
    }

    return 0;
}
