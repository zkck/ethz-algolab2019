#include <iostream>

// BGL
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>

// BFS
#include <queue>

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

        graph G(n);
        for (auto eit = T.all_edges_begin(); eit != T.all_edges_end(); eit++) {
            if (T.is_infinite(eit)) continue;
            if (T.segment(eit).squared_length() <= p) {
                info v1 = eit->first->vertex(0)->info();
                info v2 = eit->first->vertex(1)->info();
                boost::add_edge(v1, v2, G);
            }
        }

        std::vector<int> component_map(n);
        int ncc = boost::connected_components(G, 
            boost::make_iterator_property_map(component_map.begin(), 
                boost::get(boost::vertex_index, G)));

        for (size_t i = 0; i < m; i++)
        {
            int x0, y0, x1, y1; std::cin >> x0 >> y0 >> x1 >> y1;
            Point s(x0, y0), t(x1, y1);

            Delaunay::Vertex_handle src = T.nearest_vertex(s);
            Delaunay::Vertex_handle dst = T.nearest_vertex(t);

            int src_in_range = CGAL::squared_distance(s, src->point()) <= p / 4;
            int dst_in_range = CGAL::squared_distance(t, dst->point()) <= p / 4;

            int same_cc = component_map[src->info()] == component_map[dst->info()];

            if (src_in_range && dst_in_range && same_cc)
                std::cout << "y";
            else
                std::cout << "n"; 

            // // IDEA 1: BFS
            // //         ===

            // std::vector<int> visited(n, false);
            // std::queue<Delaunay::Vertex_handle> Q;
            
            // // source is the nearest vertex to start of mission
            // Delaunay::Vertex_handle src = T.nearest_vertex(s);
            // if (CGAL::squared_distance(s, src->point()) <= p / 4)
            // {
            //     Q.push(src);
            //     visited[src->info()] = true;
            // }

            // // destination is the nearest vertex to end of mission
            // Delaunay::Vertex_handle dst = T.nearest_vertex(t);
            // int success = CGAL::squared_distance(t, dst->point()) <= p / 4
            //     ? -1    // reachable, but have to check with BFS
            //     :  0;   // unreachable

            // // while BFS is not complete and that the destination has not been found
            // while (!Q.empty() && success < 0) {
            //     const Delaunay::Vertex_handle u = Q.front();
            //     Q.pop();

            //     if (u->info() == dst->info()) {
            //         success = 1; // reachable
            //     }

            //     Delaunay::Vertex_circulator v = u->incident_vertices();
            //     do
            //     {
            //         if (T.is_infinite(v)) continue;
            //         long dist = CGAL::squared_distance(u->point(), v->point());
            //         if (dist <= p && !visited[v->info()]) {
            //             Q.push(v);
            //             visited[v->info()] = true;
            //         }
            //     } while (++v != u->incident_vertices());
            // }

            // if (success == 1)
            //     std::cout << "y";
            // else
            //     std::cout << "n";
        }

        std::cout << std::endl;

        // first testset, b=p and a=4p
        std::cout << 4 * p << std::endl;
        std::cout << p << std::endl;
    }
    
    return 0;
}
