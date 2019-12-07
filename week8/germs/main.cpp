#include <iostream>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt EK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;

typedef Triangulation::Edge_iterator    Edge_iterator;
typedef Triangulation::Vertex_iterator  Vertex_iterator;

int to_ceil(EK::FT n) {
    int res = std::ceil(CGAL::to_double(n));
    while (res < n)      res++;
    while (res - 1 >= n) res--;
    return res;
}

int to_dist(IK::FT sq_dist)
{
    EK::FT fuck = CGAL::sqrt(sq_dist);
    EK::FT thss = fuck / 2 - 0.5;
    EK::FT shit = CGAL::sqrt(fuck / 2.0 - 0.5);
    return to_ceil(shit);
}

void algo(int n)
{
    int l, b, r, t; std::cin >> l >> b >> r >> t;

    int width = r - l, height = t - b;

    std::vector<IK::Point_2> pts;
    pts.reserve(n);
    
    for (size_t i = 0; i < n; i++)
    {
        int x, y; std::cin >> x >> y;
        pts.push_back(IK::Point_2(x - l, y - b));
    }

    Triangulation tri;
    tri.insert(pts.begin(), pts.end());

    // death times for each vertex
    std::vector<IK::FT> death_times;

    for (Vertex_iterator v = tri.finite_vertices_begin(); v != tri.finite_vertices_end(); ++v)
    {
        IK::FT v_min_x = v->point().x();
        v_min_x = (width < 2 * v_min_x) ? width - v_min_x : v_min_x;

        IK::FT v_min_y = v->point().y();
        v_min_y = (height < 2 * v_min_y) ? height - v_min_y : v_min_y;
        
        IK::FT v_min = v_min_y < v_min_x ? v_min_y : v_min_x;
        IK::FT death = v_min * v_min * 2;

        Triangulation::Edge_circulator c = tri.incident_edges(v);

        if (c != 0)
            do
            {
                if (tri.is_infinite(c)) continue;
                IK::FT length = tri.segment(c).squared_length();
                if (length < death) death = length;
            } while (++c != tri.incident_edges(v));
        
        death_times.push_back(death);
    }

    std::sort(death_times.begin(), death_times.end());

    IK::FT half = death_times[n / 2];
    IK::FT min  = death_times[0];
    IK::FT max  = death_times[n - 1];

    printf("%d %d %d\n", to_dist(min), to_dist(half), to_dist(max));


    
}
int main(int argc, char const *argv[])
{
    int n;
    for (std::cin >> n; n != 0; std::cin >> n)  algo(n);
    return 0;
}
