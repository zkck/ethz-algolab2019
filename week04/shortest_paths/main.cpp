#include <iostream>
#include <limits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

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
    int n, m, q; std::cin >> n >> m >> q;
    int lons[n], lats[n];
    for (int v = 0; v < n; ++v) {
        std::cin >> lons[v] >> lats[v];
    }

    weighted_graph map(n);
    weight_map weights = boost::get(boost::edge_weight, map);

    for (int e = 0; e < m; ++e) {
        int a, b, c; std::cin >> a >> b >> c;
        edge_desc added = boost::add_edge(a, b, map).first;
        weights[added] = c;
    }

    for (int i = 0; i < q; ++i) {
        int s, t; std::cin >> s >> t;
        int dist = dijkstra_dist(map, s, t);
        if (dist == std::numeric_limits<int>::max())
            std::cout << "unreachable" << std::endl;
        else
            std::cout << dist << std::endl;
    }

    return 0;
}
