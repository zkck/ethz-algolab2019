#include <iostream>
#include <vector>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;

typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;

typedef Triangulation::Finite_faces_iterator  Face_iterator;

void algo(int n)
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

    int m; std::cin >> m;

    for (size_t i = 0; i < m; i++)
    {
        int x, y; std::cin >> x >> y;
        IK::Point_2 point(x, y);

        auto nearest = t.nearest_vertex(point);
        auto dist = CGAL::squared_distance(point, nearest->point());

        std::cout << (long) CGAL::to_double(dist) << std::endl;
    }
}

int main(int argc, char const *argv[])
{
    std::ios_base::sync_with_stdio(false);
    int n;
    while (std::cin >> n && n > 0) algo(n);
    return 0;
}
