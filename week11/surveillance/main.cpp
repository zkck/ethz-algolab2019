#include <iostream>
#include <vector>

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
        // - n is number of intersections
        // - m is number of streets
        // - k is number of police stations
        // - l is number of photographs
        int n, m, k, l; std::cin >> n >> m >> k >> l;

        graph G(2 + 2 * n);
        edge_adder adder(G);

        int src = 0;
        int dst = 2 * n + 1;

        // police stations are at intersections
        int police_station;
        for (size_t i = 0; i < k; i++) {
            std::cin >> police_station;
            // connect in the graph and in graph mirror
            adder.add_edge(src, 1 + police_station, 1);     // one policeman
            adder.add_edge(1 + police_station + n, dst, 1); // one safe
        }

        // photographs are at intersections
        int photograph;
        for (size_t i = 0; i < l; i++) {
            std::cin >> photograph;
            // the photograph connects graph and mirror
            adder.add_edge(1 + photograph, 1 + n + photograph, 1);
        }

        // streets connect intersections
        for (size_t i = 0; i < m; i++) {
            int x, y; std::cin >> x >> y;
            // add edge in graph and mirror
            adder.add_edge(1 + x, 1 + y, std::numeric_limits<long>::max());
            adder.add_edge(1 + x + n, 1 + y + n, 1);
        }

        // IDEA 1
        //
        // Police stations are connected to the source/sink
        // Going over a photograph vertex gives a reward?

        // IDEA 2 <-- selected
        //
        // Have 2 graphs, one for the mirror (retrieval of the photograph)

        std::cout << boost::push_relabel_max_flow(G, src, dst) << std::endl;



    }
    return 0;
}
