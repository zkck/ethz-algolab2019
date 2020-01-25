#include <iostream>
#include <vector>
#include <queue>

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
    while (num_tests-- > 0) {
        int Z, J; std::cin >> Z >> J;

        graph G(2 + J + Z);
        edge_adder adder(G);
        auto rc_map = boost::get(boost::edge_residual_capacity, G);

        int src = 0, sink = J + Z + 1;

        // ticket costs
        int costs[Z];
        for (size_t i = 0; i < Z; ++i) {
            std::cin >> costs[i];
            adder.add_edge(1 + J + i, sink, costs[i]);
        }

        // job rewards
        int rewards[J];
        for (size_t j = 0; j < J; ++j) {
            std::cin >> rewards[j];
            adder.add_edge(src, 1 + j, rewards[j]);
        }

        // job zones
        const int INF = std::numeric_limits<int>::max();
        for (size_t j = 0; j < J; ++j) {
            int N; std::cin >> N;
            for (size_t i = 0; i < N; i++) {
                int zone; std::cin >> zone;
                adder.add_edge(1 + j, 1 + J + zone, INF);
            }

        }

        // GOAL
        //
        // Maximize the profit: the rewards of the chosen jobs - the cost of the zones

        // Per job:
        //   - either take the job : profit=profit[job]             loss=cost_tickets[job]
        //   - do not take the job:  profit=cost_no_tickets[job]    loss=profit[job]

        // Formulating the problem with min-cut
        //
        // Firstly, consider two layers: one for the jobs, and another for the zones.
        //
        // We connect the source to the jobs with an edge capacity that corresponds to the profit
        // of the job.
        //
        // We connect the zones to the sink with an edge capacity corresponding to the cost of
        // the zone's ticket.
        //
        // We want to make the jobs and its zones inseparable, i.e. they cannot be disconnected
        // in the min-cut. This means that we need to connect them with an edge of an infinitely
        // large capacity.
        //
        // The min-cut algorithm (max-flow) will choose the minimum between paying for a job
        // and paying for its tickets. The jobs/zones in the min-cut are those that have been
        // selected.

        // Find a min cut via maxflow
        int flow = boost::push_relabel_max_flow(G, src, sink);

        // BFS to find vertex set S
        std::vector<int> vis(2 + J + Z, false); // visited flags
        std::queue<int> Q; // BFS queue (from std:: not boost::)
        vis[src] = true; // Mark the source as visited
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
                        Q.push(v);
                }
        }

        int total = 0;
        for (int i = 0; i < 2 + J + Z; ++i) {
            if (!vis[i]) continue;
            if (1 <= i && i <= J)
                total += rewards[i - 1];
            if (J < i && i <= J + Z)
                total -= costs[i - J - 1];
        }
        std::cout << total << std::endl;




    }
    return 0;
}
