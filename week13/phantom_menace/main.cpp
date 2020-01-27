#include <iostream>
#include <limits>

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

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
        int n, m, s, d; std::cin >> n >> m >> s >> d;
        graph G(2 * n);
        edge_adder adder(G);
        for (size_t i = 0; i < n; i++)
        {
            // vertex capacities
            adder.add_edge(i, n + i, 1);
        }


        vertex_desc v_source = boost::add_vertex(G);
        vertex_desc v_target = boost::add_vertex(G);

        for (size_t obj = 0; obj < m; obj++)
        {
            int i, j; std::cin >> i >> j;
            adder.add_edge(i + n, j, 1);
        }

        const int INF = std::numeric_limits<int>::max();

        for (size_t i = 0; i < s; i++)
        {
            int p; std::cin >> p;
            adder.add_edge(v_source, p, INF);
        }

        for (size_t i = 0; i < d; i++)
        {
            int p; std::cin >> p;
            adder.add_edge(p + n, v_target, INF);
        }

        std::cout << boost::push_relabel_max_flow(G, v_source, v_target) << std::endl;

    }
    return 0;
}
