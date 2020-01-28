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

        // std::cout << "  parsing..." << std::endl;

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

        // std::cout << "  mapping..." << std::endl;


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

        // IDEA
        //
        // MCMF with each layer corresponding to a time, each flow unit corresponding to a car,
        // each job a

        // std::cout << "  building..." << std::endl;

        graph G(S * times.size());
        auto c_map = boost::get(boost::edge_capacity, G);
        auto w_map = boost::get(boost::edge_weight, G); // new!
        edge_adder adder(G);
        vertex_desc source = boost::add_vertex(G);
        vertex_desc sink   = boost::add_vertex(G);

        const int INF = std::numeric_limits<int>::max();

        for (size_t i = 0; i < S; ++i) {
            adder.add_edge(source, i, l[i], 0);
            for (size_t j = 0; j < times.size() - 1; j++) {
                adder.add_edge(j * S + i, (j + 1) * S + i, INF, COST_COMP);
            }
            adder.add_edge((times.size() - 1) * S + i, sink, INF, 0);
        }

        edge_desc edge; int flag;
        for (auto &req : requests) {
            size_t t1 = mapping[req.d];
            size_t t2 = mapping[req.a];

            size_t u = t1 * S + req.s;
            size_t v = t2 * S + req.t;
            int cost = (t2 - t1) * COST_COMP - req.p;

            boost::tie(edge, flag) = boost::edge(u, v, G);
            if (flag && w_map[edge] == cost)
                c_map[edge]++;
            else
                adder.add_edge(u, v, 1, cost);
        }

        printf("  m=%ld n=%ld\n", boost::num_edges(G), boost::num_vertices(G));

        // Option 2: Min Cost Max Flow with successive_shortest_path_nonnegative_weights
        boost::successive_shortest_path_nonnegative_weights(G, source, sink);
        int cost2 = boost::find_flow_cost(G);
        std::cout << -(cost2 - num_cars * (times.size() - 1) * COST_COMP) << "\n";


    }

    return 0;
}
