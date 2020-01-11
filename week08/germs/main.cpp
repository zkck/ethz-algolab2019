#include <iostream>
#include <limits>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Delaunay_triangulation_2<IK>  Triangulation;

typedef Triangulation::Edge_iterator    Edge_iterator;
typedef Triangulation::Vertex_iterator  Vertex_iterator;

int to_dist(double dt) { return std::ceil(std::sqrt(std::sqrt(dt) - 0.5)); }

void algo(int n)
{
    int l, b, r, t; std::cin >> l >> b >> r >> t;

    int width = r - l, height = t - b;

    std::vector<IK::Point_2> pts;
    pts.reserve(n);

    for (size_t i = 0; i < n; i++)
    {
        int x, y; std::cin >> x >> y;
        pts.push_back(IK::Point_2(x, y));
    }

    Triangulation tri;
    tri.insert(pts.begin(), pts.end());

    // death times for each vertex
    std::vector<double> death_times;

    for (Vertex_iterator u = tri.finite_vertices_begin(); u != tri.finite_vertices_end(); ++u)
    {
        double dt = std::numeric_limits<double>::infinity();
        dt = std::min(dt, CGAL::squared_distance(u->point(), IK::Point_2(l, u->point().y())));
        dt = std::min(dt, CGAL::squared_distance(u->point(), IK::Point_2(r, u->point().y())));
        dt = std::min(dt, CGAL::squared_distance(u->point(), IK::Point_2(u->point().x(), b)));
        dt = std::min(dt, CGAL::squared_distance(u->point(), IK::Point_2(u->point().x(), t)));

        Triangulation::Vertex_circulator v = tri.incident_vertices(u);
        if (v != 0)
            do
            {
                if (tri.is_infinite(v)) continue;
                // for the equivalent of a division of the distance by 2, we need to divide
                // the squared distance by 4
                dt = std::min(dt, CGAL::squared_distance(u->point(), v->point()) / 4);
            } while (++v != tri.incident_vertices(u));

        death_times.push_back(dt);
    }

    if (death_times.size() != n) std::cout << "not equal" << std::endl;

    std::sort(death_times.begin(), death_times.end());

    double half = death_times[n / 2];
    double min  = death_times[0];
    double max  = death_times[n - 1];

    printf("%d %d %d\n", to_dist(min), to_dist(half), to_dist(max));
}

int main(int argc, char const *argv[])
{
    int n;
    for (std::cin >> n; n != 0; std::cin >> n)  algo(n);
    return 0;
}
