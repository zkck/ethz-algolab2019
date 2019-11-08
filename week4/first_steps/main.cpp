#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property,                         // no vertex property
  boost::property<boost::edge_weight_t, int>  // interior edge weight property
> weighted_graph;

typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type
    weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor
    edge_desc;

std::vector<edge_desc> kruskal_results(weighted_graph& g) {    
    std::vector<edge_desc> mst;
    boost::kruskal_minimum_spanning_tree(g, std::back_inserter(mst));
    return mst;
}

int dijkstra_dist(const weighted_graph &G, int s) {
  int n = boost::num_vertices(G);
  std::vector<int> dist_map(n);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  int max_dist = 0;
  for (auto it = dist_map.begin(); it != dist_map.end(); ++it) {
      if (*it > max_dist) max_dist = *it;
  }

  return max_dist;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int n, m; std::cin >> n >> m;

        weighted_graph g(n);
        weight_map weights = boost::get(boost::edge_weight, g);

        for (int i = 0; i < m; ++i) {
            int v1, v2, w; std::cin >> v1 >> v2 >> w;
            edge_desc e = boost::add_edge(v1, v2, g).first;
            weights[e] = w;
        }

        std::vector<edge_desc> mst = kruskal_results(g);

        int sum = 0;
        for (auto it = mst.begin(); it != mst.end(); ++it) {
            sum += weights[*it];
        }

        std::cout << sum << " " << dijkstra_dist(g, 0) << std::endl;
    }
    return 0;
}
