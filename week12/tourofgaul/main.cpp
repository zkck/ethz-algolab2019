// Includes
// ========
#include <iostream>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

// BFS
#include <queue>

// Graph Type with nested interior edge properties for Cost Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor,
                boost::property <boost::edge_weight_t, long> > > > > graph;

typedef boost::graph_traits<graph>::edge_descriptor             edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator           out_edge_it; // Iterator

typedef boost::graph_traits<graph>::edge_iterator edge_it;

#define UPPER_BOUND (1<<7)


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

struct fruit {
    int a;
    int b;
    int d;
};

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n, m; std::cin >> n >> m;

        int N = n + 2;

        int v_source = N - 2;
        int v_target = N - 1;

        graph G(N);
        edge_adder adder(G);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto r_map = boost::get(boost::edge_reverse, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        auto rc_map = boost::get(boost::edge_residual_capacity, G);

        // Add the edges
        int sum_caps = 0;
        int prev = 0;
        for (size_t i = 0; i < n - 1; i++)
        {
            int cap; std::cin >> cap;
            adder.add_edge(i, i + 1, cap, UPPER_BOUND);
            sum_caps += cap;
            int diff = cap - prev;
            if (diff > 0) {
                adder.add_edge(v_source, i, diff, 0);
            } else if (diff < 0) {
                adder.add_edge(i, v_target, -diff, 0);
            }
            prev = cap;
        }
        adder.add_edge(n - 1, v_target, prev, 0);

        for (size_t i = 0; i < m; i++)
        {
            int a, b, d; std::cin >> a >> b >> d;
            int cost = -d + (b - a) * UPPER_BOUND;
            // check if fruit exists
            int exists = false;
            out_edge_it ebeg, eend;
            boost::tie(ebeg, eend) = boost::out_edges(a, G);
            for (; !exists && ebeg != eend; ebeg++) {
                if (boost::target(*ebeg, G) == b && w_map[*ebeg] == cost) {
                    c_map[*ebeg]++;
                    exists = true;
                }
            }
            if (!exists)
                adder.add_edge(a, b, 1, cost);
        }

        boost::successive_shortest_path_nonnegative_weights(G, v_source, v_target);
        long cost2 = boost::find_flow_cost(G);

        out_edge_it ebeg, eend;

        // Adjusting the cost

        // The free spots that enter the stop chain at a certain vertex
        // v will go over n - 1 - v stops, meaning that we need to reduce the cost by that
        // amount times the upper bound.
        for (boost::tie(ebeg, eend) = boost::out_edges(v_source, G); ebeg != eend; ebeg++) {
            int v = boost::target(*ebeg, G);
            assert(0 <= v && v < n);
            cost2 -= (n - 1 - v) * (c_map[*ebeg] - rc_map[*ebeg]) * UPPER_BOUND;
        }

        // The free spots that exit the stop chain at a certain vertex v will not go
        // over n - 1 - v stops, meaning that we need to readjust the cost by incresing the cost
        // by the number of unvisited stops times the upper bound.
        for (boost::tie(ebeg, eend) = boost::out_edges(v_target, G); ebeg != eend; ebeg++) {
            int v = boost::target(*ebeg, G);
            assert(0 <= v && v < n);
            cost2 += (n - 1 - v) * (rc_map[*ebeg] - c_map[*ebeg]) * UPPER_BOUND;
        }



        std::cout << -cost2 << std::endl;

    }

    return 0;
}
