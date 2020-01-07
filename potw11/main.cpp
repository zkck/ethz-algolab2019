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
  edge_desc add_edge(int from, int to, long capacity, long cost) {
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
    return e;
  }
};

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        // c - number of cities
        // g - number of guides
        // b - total budget to spend on luggage transport
        // k - the source city
        // a - the destination city
        long c, g, b, k, a; std::cin >> c >> g >> b >> k >> a;

        graph G(c + 1);
        edge_adder adder(G);

        for (size_t i = 0; i < g; i++)
        {
            // guide that
            //   - owns e elephants
            //   - is willing to send them with one suitcase from x to y
            //   - at the cost of d per elephant
            long x, y, d, e; std::cin >> x >> y >> d >> e;

            adder.add_edge(x + 1, y + 1, e, d);
        }

        // what is the maximum number of suitcases he can send
        // without exceeding his budget

        int flow = boost::push_relabel_max_flow(G, k + 1, a + 1);
        boost::cycle_canceling(G);
        int cost = boost::find_flow_cost(G);

        if (cost <= b)
            std::cout << flow << std::endl;
        else
        {
            auto c_map = boost::get(boost::edge_capacity, G);
            edge_desc e = adder.add_edge(0, k + 1, 0, 0);

            // binary search for the best

            int lower = 0, upper = 2;

            // std::cout << "binary search..." << std::endl;

            while (upper - lower > 1)
            {
                // std::cout << "  lower=" << lower << " upper=" << upper << std::endl;
                int capacity = (lower + upper) / 2;
                c_map[e] = capacity;

                int fuck = boost::push_relabel_max_flow(G, 0, a + 1);
                // if (fuck > capacity)
                //     std::cout << "What the fuck" << std::endl;

                boost::cycle_canceling(G);
                if (boost::find_flow_cost(G) > b)
                    upper = fuck;
                else
                    lower = fuck;
            }

            std::cout << lower << std::endl;

        }
    }

    return 0;
}
