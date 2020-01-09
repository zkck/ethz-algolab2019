#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <limits>

// BGL include
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
                boost::property<boost::edge_residual_capacity_t, long,
                        boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >   graph;
// Interior Property Maps
typedef boost::graph_traits<graph>::edge_descriptor                     edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator                   out_edge_it;

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
    while (num_tests-- > 0)
    {
        int n, m; std::cin >> n >> m;

        graph G(n);
        edge_adder adder(G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);

        for (size_t i = 0; i < m; i++)
        {
            int a, b, c; std::cin >> a >> b >> c;
            adder.add_edge(a, b, c);
        }

        int src = -1, dst = -1;
        long best_flow = std::numeric_limits<long>::max();
        for (int i = 0; i < 1; i++) {
            for (int j = 0; j < n; j++) {
                if (i == j) continue;
                long flow = boost::push_relabel_max_flow(G, i, j);
                if (flow < best_flow) {
                    best_flow = flow;
                    src = i;
                    dst = j;
                }
            }
        }

        boost::push_relabel_max_flow(G, src, dst);

        // BFS to find vertex set S
        std::vector<int> vis(n, false); // visited flags
        std::queue<int> Q; // BFS queue (from std:: not boost::)
        vis[src] = true; // Mark the source as visited
        int num_visited = 1;
        Q.push(src);
        while (!Q.empty()) {
            const int u = Q.front();
            Q.pop();
            out_edge_it ebeg, eend;
            for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
                const int v = boost::target(*ebeg, G);
                // Only follow edges with spare capacity
                if (rc_map[*ebeg] == 0 || vis[v]) continue;
                vis[v] = true;
                num_visited++;
                Q.push(v);
            }
        }

        std::cout << best_flow << std::endl;
        std::cout << num_visited;
        for (size_t v = 0; v < n; v++) {
            if (vis[v]) std::cout << " " << v;
        }
        std::cout << std::endl;

    }

    return 0;
}