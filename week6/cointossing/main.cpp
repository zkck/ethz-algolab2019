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
        int n, m; std::cin >> n >> m;
        
        // // points per player
        // std::vector<int> points(n, 0);

        // std::vector<std::pair<int, int>> rounds;

        // for (int i = 0; i < m; ++m) {
        //     int a, b, c; std::cin >> a >> b >> c;
        //     if (c > 0)
        //         points[c == 1 ? a : c]++;
        //     else
        //         rounds.push_back(std::pair(a, b));
        // }

        // graph with source, rounds, players and sink
        graph G(n + m + 2);
        edge_adder adder(G);

        int source = 0;
        int sink = n + m + 1;

        for (int r = 0; r < m; ++r) {
            int a, b, c; std::cin >> a >> b >> c;

            int r_vertex = 1 + r;
            int a_vertex = 1 + m + a;
            int b_vertex = 1 + m + b;

            // first add the point flow to the round
            adder.add_edge(source, r_vertex, 1);

            // then add the edge to the player depending on who won
            if (c > 0)
                adder.add_edge(r_vertex, c == 1 ? a_vertex : b_vertex, 1);
            else {
                adder.add_edge(r_vertex, a_vertex, 1);
                adder.add_edge(r_vertex, b_vertex, 1);
            }
        }

        // at this point first and second layer are done

        int score_sum = 0;

        for (int p = 0; p < n; ++p) {
            int s; std::cin >> s;
            score_sum += s;

            int p_vertex = 1 + m + p;

            adder.add_edge(p_vertex, sink, s);
        }

        long flow = boost::push_relabel_max_flow(G, source, sink);

        if (score_sum == m && m == flow)
            std::cout << "yes" << std::endl;
        else
            std::cout << "no" << std::endl;

        

    }
    return 0;
}
