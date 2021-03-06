#include <iostream>
#include <set>

#define UPPER_BOUND 100000
#define COST_COMP   100

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
typedef boost::graph_traits<graph>::vertex_descriptor           vertex_desc;
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

struct request {
    int s;
    int t;
    int d;
    int a;
    int p;
};


int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        // N : number of booking requests (<= 10'000)
        // S : number of rental stations (<= 10)
        int N, S; std::cin >> N >> S;

        // l[i] : the number of cars placed at a station (<= 100)
        int num_cars = 0;
        int l[S];
        for (size_t i = 0; i < S; i++) {
            std::cin >> l[i];
            num_cars += l[i];
        }

        std::set<int> times;
        std::vector<struct request> requests;
        for (size_t i = 0; i < N; i++)
        {
            // s : starting rental station
            // t : target rental station
            // d : departure time
            // a : arrival time
            // p : profit
            int s, t, d, a, p; std::cin >> s >> t >> d >> a >> p;
            s--;
            t--;
            times.insert(d);
            times.insert(a);
            requests.push_back( { s, t, d, a, p } );
        }

        // create mapping of the times;
        std::vector<size_t> mapping(UPPER_BOUND + 1, -1);

        size_t index = 0;
        for (auto it = times.begin(); it != times.end(); it++)
        {
            mapping[*it] = index++;
        }

        // GOAL
        //
        // Maximize profit.

        // IDEA (100)
        //
        // Do path compression.
        //
        // V1
        //
        // The timestamps in one lane can be compressed to only those that are used.
        //
        // Problem: There can be edges that go backwards, making it hard to have non-negative
        //          costs.
        //
        // V2 <-- TODO
        //
        // A series of unused timestamps in a lane can be compressed by just summing up their
        // costs.


        std::vector<int> used[S];
        for (size_t i = 0; i < S; i++) {
            used[i].resize(times.size());
        }
        for (auto &req : requests) {
            size_t t1 = mapping[req.d];
            size_t t2 = mapping[req.a];
            used[req.s][t1] = true;
            used[req.t][t2] = true;
        }

        graph G;
        edge_adder adder(G);
        auto c_map = boost::get(boost::edge_capacity, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!

        vertex_desc source = boost::add_vertex(G);
        vertex_desc sink   = boost::add_vertex(G);

        const int INF = std::numeric_limits<int>::max();

        std::vector<int> associated_vertex[S];
        for (size_t i = 0; i < S; i++)
        {
            associated_vertex[i].resize(times.size(), -1);

            // first vertex of the chain
            int last = boost::add_vertex(G);
            associated_vertex[i][0] = last;
            adder.add_edge(source, last, l[i], 0);

            // join vertices of the chain
            int pending = 1;
            for (size_t j = 1; j < times.size(); j++)
            {
                if (used[i][j] || j == times.size() - 1) {
                    int node = boost::add_vertex(G);
                    associated_vertex[i][j] = node;
                    adder.add_edge(last, node, INF, pending * COST_COMP);
                    last = node;
                    pending = 1;
                } else {
                    pending++;
                }
            }

            // join chain to sink
            adder.add_edge(last, sink, INF, 0);
        }

        for (auto &req : requests) {
            size_t t1 = mapping[req.d];
            size_t t2 = mapping[req.a];

            int u = associated_vertex[req.s][t1];
            int v = associated_vertex[req.t][t2];

            assert(u != -1 && v != -1);

            int cost = (t2 - t1) * COST_COMP - req.p;

            adder.add_edge(u, v, 1, cost);
        }


        // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
        boost::successive_shortest_path_nonnegative_weights(G, source, sink);
        int cost2 = boost::find_flow_cost(G);
        std::cout << -(cost2 - num_cars * (times.size() - 1) * COST_COMP) << "\n";


    }

    return 0;
}
