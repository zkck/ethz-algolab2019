#include <iostream>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/intersections.h>
#include <math.h>
#include <stack>
#include <CGAL/number_utils.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef K::Point_2    Point_2;
typedef K::Ray_2      Ray_2;
typedef K::Segment_2  Segment_2;

double floor_to_double(const K::FT& x){  double a = std::floor(CGAL::to_double(x));  while (a > x) a -= 1;  while (a+1 <= x) a += 1;  return a;}

int main(int argc, char const *argv[])
{
    while (true) {
        long n; std::cin >> n;
        if (n == 0) break;

        long x, y, a, b;
        std::cin >> x >> y >> a >> b;

        const Point_2 src(x, y);
        const Point_2 dst(a, b);

        Ray_2 ray(src, dst);

        bool intersection = false;
        Point_2 best(0, 0);

        for (int i = 0; i < n; ++i) {
            long r, s, t, u;
            std::cin >> r >> s >> t >> u;

            // build segment
            Point_2 p1(r, s);
            Point_2 p2(t, u);

            Segment_2 segment(p1, p2);

            if (CGAL::do_intersect(segment, ray)) {
                auto result = CGAL::intersection(ray, segment);
                if (const Point_2 *p = boost::get<Point_2>(&*result)) {
                    if (!intersection || CGAL::has_smaller_distance_to_point(src, *p, best))
                        best = *p;
                } else if (const Segment_2 *s = boost::get<Segment_2>(&*result)) {
                    Point_2 p = CGAL::has_smaller_distance_to_point(src, s->source(), s->target())
                        ? s->source()
                        : s->target();
                    if (!intersection || CGAL::has_smaller_distance_to_point(src, p, best)) {
                        best = p;
                    }
                }
                intersection = true;
            }
        }

        if (!intersection)
            std::cout << "no" << std::endl;
        else {
            long x = floor_to_double(best.x());
            long y = floor_to_double(best.y());
            std::cout << x << " " << y << std::endl;
        }

    }

    return 0;
}

