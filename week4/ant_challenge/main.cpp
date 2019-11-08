#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
    boost::no_property,                             // vertex property
    boost::property<boost::edge_weight_t, int>      // edge property
> weighted_graph;

typedef boost::property_map<
    weighted_graph,
    boost::edge_weight_t
>::type weight_map;

typedef boost::graph_traits<
    weighted_graph
>::edge_descriptor edge_desc;

typedef boost::graph_traits<
    weighted_graph
>::vertex_descriptor vertex_desc;

int dijkstra_dist(const weighted_graph &G, int s, int t) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return dist_map[t];
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int n; std::cin >> n;
        int e; std::cin >> e;
        int s; std::cin >> s;
        int a, b; std::cin >> a >> b;

        std::vector<weighted_graph> graphs;
        std::vector<weight_map> weight_maps;

        // initialize variables per species
        for (int i = 0; i < s; ++i) {
            weighted_graph g(n);
            graphs.push_back(g);
            weight_maps.push_back(boost::get(boost::edge_weight, g));
        }

        for (int i = 0; i < e; ++i) {
            int t1, t2; std::cin >> t1 >> t2;
            for (int j = 0; j < s; ++j) {
                int w; std::cin >> w;
                edge_desc e = boost::add_edge(t1, t2, graphs[j]).first;
                weight_maps[j][e] = w;
            }
        }

        std::vector<int> hives;
        for (int i = 0; i < s; ++i) {
            int h; std::cin >> h;
            hives.push_back(h);
        }

        weighted_graph minimum_graph(n);
        weight_map minimum_weights = boost::get(boost::edge_weight, minimum_graph);
    
        // for each species, construct mst and 
        for (int i = 0; i < s; ++i) {
            std::vector<edge_desc> mst;
            boost::kruskal_minimum_spanning_tree(graphs[i], std::back_inserter(mst));
            for (edge_desc &e : mst) {
                int u = boost::source(e, graphs[i]);
                int v = boost::target(e, graphs[i]);
                edge_desc new_e = boost::add_edge(u, v, minimum_graph).first;
                int w = weight_maps[i][e];
                if (minimum_weights[new_e] == 0)
                    minimum_weights[new_e] = w;
                else if (minimum_weights[new_e] > w)
                    minimum_weights[new_e] = w;
            }
        }

        std::cout << dijkstra_dist(minimum_graph, a, b) << std::endl;
    }

    return 0;
}
