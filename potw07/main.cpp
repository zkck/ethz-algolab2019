#include <iostream>

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
        int n, m; std::cin >> n >> m;

        // build graph
        graph G(n + m + 2);
        edge_adder adder(G);

        // source and sink
        int source = 0;
        int sink = n + m + 1;

        for (int jedi = 1; jedi <= n; ++jedi) {

            // add power of 1 to the jedi (they can only protect one edge)
            adder.add_edge(source, jedi, 1);

            int a, b; std::cin >> a >> b;

            std::cout << a << " " << b;

            if (a <= b) {
                while (a <= b) {
                    adder.add_edge(jedi, n + a, 1);
                    std::cout << "jedi=" << jedi << " segment=" << n + a << std::endl;
                    ++a;
                }
            } else {
                for (int a1 = a; a1 <= m; ++a1) {
                    adder.add_edge(jedi, n + a1, 1);
                    std::cout << "jedi=" << jedi << " segment=" << n + a1 << std::endl;
                }
                for (int a2 = 1; a2 <= b; ++a2) {
                    adder.add_edge(jedi, n + a2, 1);
                    std::cout << "jedi=" << jedi << " segment=" << n + a2 << std::endl;
                }
            }
        }

        for (int segment = 1; segment <= m; ++segment) {
            adder.add_edge(n + segment, sink, 1);
        }

        std::cout << boost::push_relabel_max_flow(G, source, sink) << std::endl;
    }
    return 0;
}
