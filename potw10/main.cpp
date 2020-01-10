#include <iostream>

// BGL
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

#include <map>

// From CGAL docs
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <vector>

typedef size_t info;

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<info, K>        Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Delaunay;
typedef Delaunay::Point                                             Point;

typedef boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS
> graph;

typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
typedef boost::graph_traits<graph>::edge_iterator               edge_it;

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n, m; std::cin >> n >> m;
        long p; std::cin >> p;

        std::vector<std::pair<Point, info>> jammers;
        for (size_t i = 0; i < n; i++)
        {
            int x, y; std::cin >> x >> y;
            jammers.push_back(std::make_pair(Point(x, y), i));
        }

        Delaunay T;
        T.insert(jammers.begin(), jammers.end());

        // IDEA 2: CONNECTED COMPONENTS
        //         ====================

        // ordered set of the edges
        std::multimap<long, std::pair<info, info>> edges;

        graph G(n);
        for (auto vit = T.finite_vertices_begin(); vit != T.finite_vertices_end(); vit++) {
            Delaunay::Vertex_circulator uit = T.incident_vertices(vit);
            do {
                if (T.is_infinite(uit)) continue;
                long dist = CGAL::squared_distance(uit->point(), vit->point());
                info u = uit->info(), v = vit->info();
                edges.insert(std::make_pair(dist, std::make_pair(u, v))); // TODO: double
                if (dist <= p) {
                    boost::add_edge(u, v, G);
                }
            } while (++uit != T.incident_vertices(vit));
        }

        // for (auto eit = T.all_edges_begin(); eit != T.all_edges_end(); eit++) {
        //     Delaunay::Face_handle f = eit->first;
        //     int i = eit->second;
        //     Delaunay::Vertex_handle vs = f->vertex(f->cw(i));
        //     Delaunay::Vertex_handle vt = f->vertex(f->ccw(i));
        //     boost::add_edge(vs->info(), vt->info(), G);
        // }

        std::vector<int> component_map(n);
        int ncc = boost::connected_components(G,
            boost::make_iterator_property_map(component_map.begin(),
                boost::get(boost::vertex_index, G)));

        std::vector<std::pair<Point, Point>> missions;

        for (size_t i = 0; i < m; i++)
        {
            int x0, y0, x1, y1; std::cin >> x0 >> y0 >> x1 >> y1;
            Point s(x0, y0), t(x1, y1);

            missions.push_back(std::make_pair(s, t));

            Delaunay::Vertex_handle src = T.nearest_vertex(s);
            Delaunay::Vertex_handle dst = T.nearest_vertex(t);

            int src_in_range = CGAL::squared_distance(s, src->point()) <= p / 4;
            int dst_in_range = CGAL::squared_distance(t, dst->point()) <= p / 4;

            int same_cc = component_map[src->info()] == component_map[dst->info()];

            if (src_in_range && dst_in_range && same_cc)
                std::cout << "y";
            else
                std::cout << "n";
        }

        std::cout << std::endl;

        G.clear();
        for (size_t i = 0; i < n; ++i) boost::add_vertex(G);
        boost::connected_components(G,
            boost::make_iterator_property_map(component_map.begin(),
                boost::get(boost::vertex_index, G)));

        long a = 0;

        auto edge = edges.begin();

        for (auto m : missions)
        {
            Point s = m.first;
            Point t = m.second;

            Delaunay::Vertex_handle src = T.nearest_vertex(s);
            Delaunay::Vertex_handle dst = T.nearest_vertex(t);

            long src_dist = CGAL::squared_distance(s, src->point());
            long dst_dist = CGAL::squared_distance(t, dst->point());

            a = std::max(a, src_dist * 4);
            a = std::max(a, dst_dist * 4);

            info u, v;
            while (component_map[src->info()] != component_map[dst->info()]) {
                long dist = edge->first;
                std::tie(u, v) = edge->second;
                boost::add_edge(u, v, G);

                // call connected components again
                boost::connected_components(G,
                    boost::make_iterator_property_map(component_map.begin(),
                        boost::get(boost::vertex_index, G)));

                a = std::max(a, dist);
                edge++;
            }
        }



        // first testset, b=p and a=4p
        std::cout << a << std::endl;
        std::cout << p << std::endl;
    }

    return 0;
}
