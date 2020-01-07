#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

// BGL graph definitions
// =====================
// Graph Type with nested interior edge properties for Flow Algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
        boost::property<boost::edge_capacity_t, long,
                boost::property<boost::edge_residual_capacity_t, long,
                        boost::property<boost::edge_reverse_t, traits::edge_descriptor> > > >   graph;

// Interior Property Maps
typedef boost::graph_traits<graph>::edge_descriptor                     edge_desc;
typedef boost::graph_traits<graph>::out_edge_iterator                   out_edge_it;

// Custom Edge Adder Class, that holds the references
// to the graph, capacity map and reverse edge map
// ===================================================
class edge_adder {
 graph &G;

 public:
  explicit edge_adder(graph &G) : G(G) {}

  void add_edge(int from, int to, long capacity) {
    auto c_map = boost::get(boost::edge_capacity, G);
    auto r_map = boost::get(boost::edge_reverse, G);
    const edge_desc e = boost::add_edge(from, to, G).first;
    const edge_desc rev_e = boost::add_edge(to, from, G).first;
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
        int g, s, l; std::cin >> g >> s >> l;

        // four layers:
        //   - source (1)
        //   - ground stations
        //   - satellites
        //   - sink (1)
        int num_vertices = 1 + g + s + 1;

        graph G(num_vertices);
        edge_adder adder(G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);

        int source = 0, sink = num_vertices - 1;
        for (size_t i = 0; i < g; i++)
        {
            adder.add_edge(source, 1 + i, 1);
        }
        for (size_t i = 0; i < s; i++)
        {
            adder.add_edge(1 + g + i, sink, 1);
        }



        for (size_t i = 0; i < l; i++)
        {
            // ground station and satellite
            int u, v; std::cin >> u >> v;
            adder.add_edge(1 + u, 1 + g + v, 1);
        }

        // find min cut via max flow
        long flow = boost::push_relabel_max_flow(G, source, sink);
        // std::cout << "flow: " << flow << std::endl;

        // BFS to find vertex set S
        std::vector<int> vis(num_vertices, false);
        std::queue<int> Q;
        vis[source] = true;
        Q.push(source);
        while (!Q.empty())
        {
            const int u = Q.front(); Q.pop();
            out_edge_it ebeg, eend;
            boost::tie(ebeg, eend) = boost::out_edges(u, G);
            for (; ebeg != eend; ++ebeg)
            {
                const int v = boost::target(*ebeg, G);
                if (rc_map[*ebeg] == 0 || vis[v]) continue;
                vis[v] = true;
                Q.push(v);
            }
        }

        // output S
        std::vector<int> gs;
        for (size_t i = 0; i < g; i++)
        {
            if (!vis[1 + i])
                gs.push_back(i);
        }
        std::vector<int> ss;
        for (size_t i = 0; i < s; i++)
        {
            if (vis[1 + g + i])
                ss.push_back(i);
        }

        std::cout << gs.size() << " " << ss.size() << std::endl;
        for (int i : gs) std::cout << i << " ";
        for (int i : ss) std::cout << i << " ";
        if (gs.size() + ss.size() > 0)
            std::cout << std::endl;

    }
    return 0;
}
