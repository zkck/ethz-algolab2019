// Includes
// ========
#include <iostream>
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
        int n, m; std::cin >> n >> m;

        graph G(n + 2);
        edge_adder adder(G);

        const int v_source = 0;
        const int v_sink   = n + 1;

        for (size_t i = 1; i <= n - 1; i++)
        {
            int c; std::cin >> c;
            adder.add_edge(i, i + 1, c, 0);
        }

        int fruit_counts[n] = { 0 };

        for (size_t i = 0; i < m; i++)
        {
            int a, b, d; std::cin >> a >> b >> d;
            fruit_counts[a]++;
            adder.add_edge(b + 1, v_sink, 1, -d);
        }

        for (size_t i = 0; i < n; i++)
        {
            adder.add_edge(v_source, i + 1, fruit_counts[i], 0);
        }
        
        int flow = boost::push_relabel_max_flow(G, v_source, v_sink);
        boost::cycle_canceling(G);
        int cost = boost::find_flow_cost(G);

        std::cout << "cost=" << -cost << " flow=" << flow << std::endl;
        
    }
    
    return 0;
}