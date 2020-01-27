#include <iostream>
#include <vector>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

int dijkstra_dist(const weighted_graph &G, int s, std::vector<int> &dist_map) {
  int n = boost::num_vertices(G);

  boost::dijkstra_shortest_paths(G, s,
    boost::distance_map(boost::make_iterator_property_map(
      dist_map.begin(), boost::get(boost::vertex_index, G))));

  return 0;
}

// i : layer, j : index in layer
int vertex(int i, int j) {
    return i * (i + 1) / 2 + j;
}


int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int k; std::cin >> k;

        // IDEA 1:
        //
        // Call dijkstra on each corner, and choose the vertex that minimizes the sum of
        // distances.
        //
        // The built graph is a directed graph which has the weight of the destination vertex.

        int n = k * (k + 1) / 2;
        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);

        int vs[n];

        edge_desc e;
        for (size_t i = 0; i < k; i++) {
            for (size_t j = 0; j < i + 1; j++) {
                int v; std::cin >> v;

                vs[vertex(i, j)] = v;

                // add edges to all surrounding vertices

                // vertex
                int node = vertex(i, j);

                // parents
                if (i > 0) {
                    if (j > 0) {
                        e = boost::add_edge(vertex(i - 1, j - 1), node, G).first;
                        weights[e] = v;
                    }
                    if (j < i) {
                        e = boost::add_edge(vertex(i - 1, j),     node, G).first;
                        weights[e] = v;
                    }
                }

                // neighbors
                if (j > 0) {
                    e = boost::add_edge(vertex(i, j - 1), node, G).first;
                    weights[e] = v;
                }
                if (j < i) {
                    e = boost::add_edge(vertex(i, j + 1), node, G).first;
                    weights[e] = v;
                }

                // children
                if (i < k - 1) {
                    e = boost::add_edge(vertex(i + 1, j), node, G).first;
                    weights[e] = v;
                    e = boost::add_edge(vertex(i + 1, j + 1), node, G).first;
                    weights[e] = v;
                }
            }
        }


        std::vector<int> dist_map_top(n);
        std::vector<int> dist_map_left(n);
        std::vector<int> dist_map_right(n);
        dijkstra_dist(G, vertex(0, 0), dist_map_top);
        dijkstra_dist(G, vertex(k - 1, 0), dist_map_left);
        dijkstra_dist(G, vertex(k - 1, k - 1), dist_map_right);

        int min = std::numeric_limits<int>::max();
        for (size_t i = 0; i < n; i++)
        {
            int sum = dist_map_top[i] + dist_map_left[i] + dist_map_right[i] - 2 * vs[i];
            min = std::min(min, sum);
        }


        std::cout << min << std::endl;



    }

    return 0;
}
