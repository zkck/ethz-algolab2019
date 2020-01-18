#include <iostream>

#define BOUND 20

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph; // new! weightmap corresponds to costs

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

// Custom edge adder class
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}
  void add_edge(int from, int to, long capacity, long cost) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    auto w_map = boost::get(boost::edge_weight, G); // new!
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
    c_map[e] = capacity;
    c_map[rev_e] = 0; // reverse edge has no capacity!
    r_map[e] = rev_e;
    r_map[rev_e] = e;
    w_map[e] = cost;   // new assign cost
    w_map[rev_e] = -cost;   // new negative cost
  }
};

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;

        graph G(n + 2);
        edge_adder adder(G);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);


        int v_source = 0, v_sink = n + 1;

        int num_students = 0;

        for (size_t i = 0; i < n; i++)
        {
            int a, c; std::cin >> a >> c;
            adder.add_edge(v_source, 1 + i, a, c);
        }
        for (size_t i = 0; i < n; i++)
        {
            int s, p; std::cin >> s >> p;
            adder.add_edge(1 + i, v_sink, s, -p + BOUND);
            num_students += s;
        }
        for (size_t i = 0; i < n - 1; i++)
        {
            int v, e; std::cin >> v >> e;
            adder.add_edge(1 + i, 2 + i, v, e);
        }

        // long flow = boost::push_relabel_max_flow(G, v_source, v_sink);
        // boost::cycle_canceling(G);
        // long revenue = -boost::find_flow_cost(G);

        // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
        boost::successive_shortest_path_nonnegative_weights(G, v_source, v_sink);
        int revenue = -boost::find_flow_cost(G);
        int flow = 0;
        out_edge_it e, eend;
        for(boost::tie(e, eend) = boost::out_edges(boost::vertex(v_source,G), G); e != eend; ++e) {
            flow += c_map[*e] - rc_map[*e];
        }

        revenue += flow * BOUND;

        if (flow == num_students) {
            std::cout << "possible ";
        } else {
            std::cout << "impossible ";
        }

        std::cout << flow << " " << revenue << std::endl;

    }
    return 0;
}
