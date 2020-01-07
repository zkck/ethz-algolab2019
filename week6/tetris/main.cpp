#include <iostream>
#include <vector>
#include <algorithm>    // std::swap

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
        int w, n; std::cin >> w >> n;

        std::vector<int> bricks[w];

        for (int i = 0; i < n; ++i)
        {
            int l, r; std::cin >> l >> r;
            if (l > r) std::swap(l, r);
            bricks[l].push_back(r);
        }

        graph G(2 * w + n);
        edge_adder adder(G);

        // create vertex capacities for cracks

        int v_cracks[w];

        int num_vertices = 0;
        for (size_t i = 0; i < w; i++)
        {
            v_cracks[i] = num_vertices++;
            if (i > 0)
                adder.add_edge(v_cracks[i], num_vertices++, 1);

            int from = i == 0 ? v_cracks[i] : v_cracks[i] + 1;
            for (const auto &r : bricks[i])
                adder.add_edge(from, num_vertices++, 1);
        }


        int v_start = 0;
        int v_end   = 2 * w + n - 1;

        num_vertices = 0;
        for (size_t i = 0; i < w; i++)
        {
            num_vertices += i == 0 ? 1 : 2;
            for (size_t j = 0; j < bricks[i].size(); j++)
            {
                int c = bricks[i][j];
                if (c < w)
                    adder.add_edge(num_vertices++, v_cracks[c], 1);
                else
                    adder.add_edge(num_vertices++, v_end, 1);
            }
        }

        int flow = boost::push_relabel_max_flow(G, 0, v_end);

        std::cout << flow << std::endl;

    }
    return 0;
}
