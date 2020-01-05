#include <limits>
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

void dijkstra_dist(const weighted_graph &G, int s, std::vector<int> &dist_map) {
  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n, m, k, T; std::cin >> n >> m >> k >> T;
                
        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);

        // teleportation network vertices
        std::vector<int> tp_vertices;

        for (size_t i = 0; i < T; i++)
        {
            size_t tp_vertex; std::cin >> tp_vertex;
            tp_vertices.push_back(tp_vertex);
        }

        // sort for intersection later on
        std::sort(tp_vertices.begin(), tp_vertices.end());

        for (size_t i = 0; i < m; i++)
        {
            // we treat the vertices in the opposite direction
            int u, v, c; std::cin >> u >> v >> c;
            edge_desc e = boost::add_edge(v, u, G).first; weights[e] = c;
        }

        // step 1: run connected components to be able to translate the
        // teleportation network to vertices

        // Connected components
        // ====================
        std::vector<int> component_map(n);      // We MUST use such a vector as an Exterior Property Map: Vertex -> Component
        int ncc = boost::strong_components(G,
            boost::make_iterator_property_map(component_map.begin(),
            boost::get(boost::vertex_index, G)));

        // Output all the components
        // ==========================
        std::vector<std::vector<vertex_desc>> component_vertices(ncc);
        // Iterate over all vertices
        for (int i = 0; i < n; ++i)
            component_vertices[component_map[i]].push_back(i);

        // new approach: create a vertex for each scc
        //   - each vertex of the ncc && teleportation network can enter the conn comp for the cost

        weighted_graph newG(n + ncc);
        weight_map new_weights = boost::get(boost::edge_weight, newG);

        boost::graph_traits<weighted_graph>::edge_iterator edges, edges_end;
        boost::tie(edges, edges_end) = boost::edges(G);
        // add all old edges
        for (; edges != edges_end; edges++) {
            int v1 = boost::source(*edges, G);
            int v2 = boost::target(*edges, G);
            int c = weights[*edges];
            edge_desc e = boost::add_edge(v1, v2, newG).first; new_weights[e] = c;
        }

        for (size_t i = 0; i < ncc; i++)
        {
            std::vector<vertex_desc> &comp = component_vertices[i];
            std::vector<int> intersection;
            std::set_intersection(
                tp_vertices.begin(), tp_vertices.end(),
                comp.begin(), comp.end(),
                std::back_inserter(intersection)
            );
            size_t isize = intersection.size();
            for (int v : intersection) {
                edge_desc e;
                e = boost::add_edge(v, n + i, newG).first; new_weights[e] = isize - 1;
                e = boost::add_edge(n + i, v, newG).first; new_weights[e] = 0;
            }
        }

        // step 2: call dijkstra and choose the best among
        std::vector<int> dist_map(n + ncc);
        dijkstra_dist(newG, n - 1, dist_map);
        
        int smallest = std::numeric_limits<int>::max();
        for (size_t i = 0; i < k; i++)
        {
            if (dist_map[i] < smallest) smallest = dist_map[i];
        }
    
        if (smallest < 1000000)
            std::cout << smallest << std::endl;
        else
            std::cout << "no" << std::endl;
        
    }
    
    return 0;
}
