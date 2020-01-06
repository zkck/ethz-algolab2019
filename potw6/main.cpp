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
    
        // 2 * n for all the potential extra strongly connected components
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

        // Strongly connected components
        // =============================
        std::vector<int> component_map(n);
        int ncc = boost::strong_components(G,
            boost::make_iterator_property_map(component_map.begin(),
            boost::get(boost::vertex_index, G)));

        // Group the vertices
        // ==================
        std::vector<std::vector<vertex_desc>> component_vertices(ncc);

        // Iterate over all n vertices
        for (int tp_vertex : tp_vertices)
            component_vertices[component_map[tp_vertex]].push_back(tp_vertex);

        // new approach: create a vertex for each scc
        //   - each vertex of the ncc && teleportation network can enter the conn comp for the cost
        
        for (size_t i = 0; i < ncc; i++)
        {
            std::vector<vertex_desc> &comp = component_vertices[i];
            size_t isize = comp.size();
            vertex_desc scc = boost::add_vertex(G);
            for (int v : comp) {
                edge_desc e;
                e = boost::add_edge(v, scc, G).first; weights[e] = isize - 1;
                e = boost::add_edge(scc, v, G).first; weights[e] = 0;
            }
        }

        // step 2: call dijkstra and choose the best among
        std::vector<int> dist_map(n + ncc);
        dijkstra_dist(G, n - 1, dist_map);
        
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
