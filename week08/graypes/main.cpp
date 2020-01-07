#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;

typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;
typedef Triangulation::Edge_iterator    Edge_iterator;

double ceil_to_double(EK::FT v)
{
    double d = std::ceil(CGAL::to_double(v));
    while (d < v)       d++;
    while (d - 1 > v)   d--;
    return d;
}

int main(int argc, char const *argv[])
{
    int n;
    for (std::cin >> n; n > 0; std::cin >> n)
    {
        std::vector<IK::Point_2> pts;
        pts.reserve(n);
        for (size_t i = 0; i < n; i++)
        {
            int x, y; std::cin >> x >> y;
            pts.push_back(IK::Point_2(x, y));
        }
        Triangulation t;
        t.insert(pts.begin(), pts.end());

        IK::FT min;
        bool set = false;

        for (Edge_iterator e = t.finite_edges_begin();
            e != t.finite_edges_end();
            e++)
        {
            IK::FT length = t.segment(e).squared_length();
            if (!set || length < min) {
                min = length;
                set = true;
            }
        }

        EK::FT time = CGAL::sqrt(EK::FT(min)) * 50;

        std::cout << ceil_to_double(time) << std::endl;
    }
    return 0;
}
