#include <iostream>

// BGL
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

// BGL incremental connected components
#include <boost/foreach.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/incremental_components.hpp>
#include <boost/pending/disjoint_sets.hpp>

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

typedef boost::graph_traits<graph>::vertex_descriptor  Vertex;
typedef boost::graph_traits<graph>::vertices_size_type VertexIndex;

typedef VertexIndex* Rank;
typedef Vertex* Parent;



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

        // CONNECTED COMPONENTS
        // ====================

        // ordered set of the edges
        std::multimap<long, std::pair<info, info>> edges;

        // STEP 1
        //
        // Build a graph modeling the triangulation, with edges only when there is an overlap
        // in the jammers radius

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

        // STEP 2
        //
        // Run connected components

        std::vector<int> component_map(n);
        int ncc = boost::connected_components(G,
            boost::make_iterator_property_map(component_map.begin(),
                boost::get(boost::vertex_index, G)));

        // STEP 3
        //
        // For each mission, check if the closest jammers to start/finish are in the same
        // connected component

        std::vector<std::pair<Point, Point>> missions;

        // keep track of succeeded/failed missions for later
        std::vector<size_t> succeeded, failed;

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

            if (src_in_range && dst_in_range && same_cc) {
                std::cout << "y";
                succeeded.push_back(i);
            } else {
                std::cout << "n";
                failed.push_back(i);
            }
        }

        std::cout << std::endl;

        // INCREMENTAL CONNECTED COMPONENTS
        // ================================

        graph graph(n);

        std::vector<VertexIndex> rank(num_vertices(graph));
        std::vector<Vertex> parent(num_vertices(graph));

        boost::disjoint_sets<Rank, Parent> ds(&rank[0], &parent[0]);

        boost::initialize_incremental_components(graph, ds);
        boost::incremental_components(graph, ds);

        // loop variables
        Point s, t;
        auto eit = edges.begin();

        long b = 0;

        for (size_t i : succeeded)
        {
            std::tie(s, t) = missions[i];
            Delaunay::Vertex_handle src = T.nearest_vertex(s);
            Delaunay::Vertex_handle dst = T.nearest_vertex(t);
            long dist_to_src = CGAL::squared_distance(s, src->point());
            long dist_to_dst = CGAL::squared_distance(t, dst->point());
            b = std::max(b, dist_to_src * 4);
            b = std::max(b, dist_to_dst * 4);

            // loop variables
            size_t u, v;
            while (ds.find_set(src->info()) != ds.find_set(dst->info())) {
                long dist = eit->first;
                std::tie(u, v) = eit->second;
                boost::add_edge(u, v, graph);
                ds.union_set(u, v);
                b = std::max(b, dist);
                eit++;
            }

        }

        long a = b;

        for (size_t i : failed)
        {
            std::tie(s, t) = missions[i];
            Delaunay::Vertex_handle src = T.nearest_vertex(s);
            Delaunay::Vertex_handle dst = T.nearest_vertex(t);
            long dist_to_src = CGAL::squared_distance(s, src->point());
            long dist_to_dst = CGAL::squared_distance(t, dst->point());
            b = std::max(a, dist_to_src * 4);
            b = std::max(a, dist_to_dst * 4);

            // loop variables
            size_t u, v;

            while (ds.find_set(src->info()) != ds.find_set(dst->info())) {
                long dist = eit->first;
                std::tie(u, v) = eit->second;
                boost::add_edge(u, v, graph);
                ds.union_set(u, v);
                b = std::max(a, dist);
                eit++;
            }

        }


        // first testset, b=p and a=4p
        std::cout << 4 * p << std::endl;
        std::cout << b << std::endl;
    }

    return 0;
}
