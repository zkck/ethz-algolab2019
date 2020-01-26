#include <iostream>
#include <map>
#include <algorithm>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<size_t, K>      Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Delaunay;
typedef Delaunay::Point                                             Point;


int connect(boost::disjoint_sets_with_storage<> &uf, Delaunay &T, int r) {
    for (auto uit = T.finite_vertices_begin(); uit != T.finite_vertices_end(); ++uit) {
        auto vit = T.incident_vertices(uit);
        do
        {
            if (T.is_infinite(vit)) continue;
            if (CGAL::squared_distance(uit->point(), vit->point()) <= r * r)
                uf.link(uit->info(), vit->info());
        } while (++vit != T.incident_vertices(uit));
    }
    return 0;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int n, m, r; std::cin >> n >> m >> r;

        // IDEA 1
        //
        // To color the graph (we have one color for each frequency), we start with the vertex
        // with the highest degree. We color it arbitrarily, and color those in his range with
        // the other color.

        std::vector<std::pair<Point, size_t>> radio_stations;
        for (size_t i = 0; i < n; i++)
        {
            int x, y; std::cin >> x >> y;
            radio_stations.push_back(std::make_pair(Point(x, y), i));
        }

        std::vector<std::pair<Point, Point>> clues;
        for (size_t i = 0; i < m; i++)
        {
            int x1, y1, x2, y2; std::cin >> x1 >> y1 >> x2 >> y2;
            clues.push_back(std::make_pair(Point(x1, y1), Point(x2, y2)));
        }


        Delaunay T;
        T.insert(radio_stations.begin(), radio_stations.end());

        // TODO
        int valid = true;

        boost::disjoint_sets_with_storage<> uf(n);
        connect(uf, T, r);


        for (auto &clue : clues) {

            if (!valid) {
                std::cout << "n";
                continue;
            }

            Point a = clue.first;
            Point b = clue.second;

            if (CGAL::squared_distance(a, b) <= r * r) {
                std::cout << "y";
                continue;
            }

            auto vh_a = T.nearest_vertex(a);
            auto vh_b = T.nearest_vertex(b);

            int a_in = CGAL::squared_distance(a, vh_a->point()) <= r * r;
            int b_in = CGAL::squared_distance(b, vh_b->point()) <= r * r;

            if (a_in && b_in) {
                if (uf.find_set(vh_a->info()) == uf.find_set(vh_b->info()))
                    std::cout << "y";
                else
                    std::cout << "n";
            } else {
                std::cout << "n";
            }
        }
        std::cout << std::endl;


    }
    return 0;
}
