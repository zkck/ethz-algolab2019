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
typedef boost::graph_traits<graph>::vertex_descriptor                   vertex_desc;
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


        // IDEA 1
        //
        // Doing a max flow on a certain source and sink will give the minimum cut that has
        // the source on the "right" set and the sink on the "left" set (0).
        //
        // We then need to compare versus the source and sink being to together on the "left"
        // side (1), on the "right" side (2), and when they are switched (3).

        long min_flow = std::numeric_limits<long>::max();
        int min_source = -1, min_sink = -1;

        // We will arbitrarily choose the source and sink to be 0 and 1 respectively.
        int source = 0, sink = 1;

        // Basic, source on right and sink on left (0)
        long base_flow      = boost::push_relabel_max_flow(G, source, sink);
        if (base_flow < min_flow) {
            min_flow = base_flow;
            min_source = source;
            min_sink   = sink;
        }

        // Inverted, sink on right and source on left (3)
        long inverted_flow  = boost::push_relabel_max_flow(G, sink, source);
        if (inverted_flow < min_flow) {
            min_flow = inverted_flow;
            min_source = sink;
            min_sink   = source;
        }

        // Source and sink on left (1)
        // ===========================

        vertex_desc aux_source = boost::add_vertex(G);

        long inf = std::numeric_limits<int>::max();
        adder.add_edge(aux_source, source, inf);
        adder.add_edge(aux_source, sink,   inf);

        for (size_t v = 2; v < n; v++) {
            int flow = boost::push_relabel_max_flow(G, aux_source, v);
            if (flow < min_flow) {
                min_flow = flow;
                min_source = source;
                min_sink   = v;
            }
        }

        // Clear the auxiliary vertex
        boost::clear_vertex(aux_source, G);

        // Source and sink on the right (2)
        // ================================

        vertex_desc aux_sink = boost::add_vertex(G);
        adder.add_edge(source, aux_sink, inf);
        adder.add_edge(sink,   aux_sink, inf);

        for (size_t v = 2; v < n; v++) {
            long flow = boost::push_relabel_max_flow(G, v, aux_sink);
            if (flow < min_flow) {
                min_flow = flow;
                min_source = v;
                min_sink   = sink;
            }
        }

        // Clear the auxiliary vertex
        boost::clear_vertex(aux_sink, G);

        // Calculate minimum
        int flow = boost::push_relabel_max_flow(G, min_source, min_sink);

        // BFS to find vertex set S
        std::vector<int> vis(n, false); // visited flags
        std::queue<int> Q; // BFS queue (from std:: not boost::)
        vis[min_source] = true; // Mark the source as visited
        int num_vertices = 1;
        Q.push(min_source);
        while (!Q.empty()) {
            const int u = Q.front();
            Q.pop();
            out_edge_it ebeg, eend;
            for (boost::tie(ebeg, eend) = boost::out_edges(u, G); ebeg != eend; ++ebeg) {
                const int v = boost::target(*ebeg, G);
                // Only follow edges with spare capacity
                if (rc_map[*ebeg] == 0 || vis[v]) continue;
                vis[v] = true;
                num_vertices++;
                Q.push(v);
            }
        }

        std::cout << flow << std::endl;

        // Output S
        std::cout << num_vertices;
        for (int i = 0; i < n; ++i) {
                if (vis[i]) std::cout << " " << i;
        }
        std::cout << "\n";


    }

    return 0;
}
