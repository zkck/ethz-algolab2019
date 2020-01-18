#include <iostream>

#include <vector>
// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// DIJKSTRA
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

// FLOW
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

int dijkstra_dist(const weighted_graph &G, int s, std::vector<int> &dist_map) {
    int n = boost::num_vertices(G);

    boost::dijkstra_shortest_paths(G, s,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));

    return 0;
}
int dijkstra_path(const weighted_graph &G, int s, std::vector<int> &dist_map, std::vector<vertex_desc> &pred_map) {
    int n = boost::num_vertices(G);

    boost::dijkstra_shortest_paths(G, s,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G)))
        .predecessor_map(boost::make_iterator_property_map(
            pred_map.begin(), boost::get(boost::vertex_index, G))));
    return 0;
}

// Custom edge adder class, highly recommended
class edge_adder {
  graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const auto e = boost::add_edge(from, to, G).first;
    const auto rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
  }
};

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int n, m, s, f; std::cin >> n >> m >> s >> f;

        int cap[n][n];

        // PROMBLEM 1
        //
        // Find all shortest paths between a source and destination

        // IDEA 1
        //
        // Run Dijktra on both the source and the destination. Then for find the vertices in the
        // graph for which the distance to vertex from source + distance to vertex from
        // destination is minimal.

        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);

        edge_desc e;
        for (size_t i = 0; i < m; i++)
        {
            // a : intersection
            // b : intersection
            // c : width (capacity)
            // d : length
            int a, b, c, d; std::cin >> a >> b >> c >> d;
            e = boost::add_edge(a, b, G).first; weights[e] = d;
            cap[a][b] = c;
            cap[b][a] = c;
        }

        std::vector<vertex_desc> pred_map(n);
        std::vector<int> dist_map(n);
        dijkstra_path(G, s, dist_map, pred_map);

        std::vector<vertex_desc> pred_map_f(n);
        std::vector<int> dist_map_f(n);
        dijkstra_path(G, f, dist_map_f, pred_map_f);

        int min_dist = std::numeric_limits<int>::max();
        std::vector<int> via_vertices;
        for (int i = 0; i < n; i++)
        {
            int dist = dist_map[i] + dist_map_f[i];
            if (dist < min_dist) {
                min_dist = dist;
                via_vertices.clear();
            }
            if (dist == min_dist) via_vertices.push_back(i);
        }

        for (int via_vertex : via_vertices)
        {
            /* code */
        }

    }
    return 0;
}
