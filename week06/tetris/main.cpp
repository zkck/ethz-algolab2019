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

        graph G(2 * w);
        edge_adder adder(G);

	int src = 0;
	int dst = 2 * w - 1;

	int num_cracks = w - 1;
	
        // create vertex capacities for cracks
	for (int i = 1; i < w; ++i)
		adder.add_edge(i, i + w - 1, 1);

	for (int l = 0; l < w; ++l) {
		for (int r : bricks[l]) {
			int from = l, to = r;
			if (l > 0) from += num_cracks;
			if (r == w) to = dst;
			adder.add_edge(from, to, 1);
		}
	}

        int flow = boost::push_relabel_max_flow(G, 0, 2 * w - 1);

        std::cout << flow << std::endl;

    }
    return 0;
}
