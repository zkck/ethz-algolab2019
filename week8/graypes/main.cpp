#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator    Edge_iterator;
typedef Triangulation::Vertex_iterator  Vertex_iterator;

int main(int argc, char const *argv[])
{
    int n;
    for (std::cin >> n; n > 0; std::cin >> n)
    {
        std::vector<K::Point_2> pts;
        pts.reserve(n);
        for (size_t i = 0; i < n; i++)
        {
            int x, y; std::cin >> x >> y;
            pts.push_back(K::Point_2(x, y));
        }
        Triangulation t;
        t.insert(pts.begin(), pts.end());

        K::FT lol;
        bool set = false;
        
        for (Edge_iterator e = t.finite_edges_begin();
            e != t.finite_edges_end();
            e++)
        {
            K::FT length = t.segment(e).squared_length();
            if (!set || length < lol) {
                lol = length;
                set = true;
            }
        }

        double d = CGAL::to_double(lol);

        std::cout << (int) std::ceil(std::sqrt(d) / 2 * 100) << std::endl;
    }
    return 0;
}
