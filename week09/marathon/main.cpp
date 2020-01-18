#include <iostream>
#include <stack>
#include <vector>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

// DIJKSTRA
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
  boost::no_property, boost::property<boost::edge_weight_t, int> >      weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor            edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor          vertex_desc;

// FLOW
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
    boost::property<boost::edge_capacity_t, long,
        boost::property<boost::edge_residual_capacity_t, long,
            boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>> graph;

// DFS
typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;

int dijkstra_dist(const weighted_graph &G, int s, std::vector<int> &dist_map) {
    int n = boost::num_vertices(G);

    boost::dijkstra_shortest_paths(G, s,
        boost::distance_map(boost::make_iterator_property_map(
            dist_map.begin(), boost::get(boost::vertex_index, G))));

    return 0;
}

// Custom edge adder class, highly recommended
class edge_adder
{
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity)
    {
        // ignore if edge exists
        if (boost::edge(from, to, G).second) return;
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
        int n, m, s, f; std::cin >> n >> m >> s >> f;

        std::vector<int> cap[n];
        for (auto &vect : cap) vect.resize(n, -1);

        // PROMBLEM 1
        //
        // Find all shortest paths between a source and destination

        // IDEA 1
        //
        // Run Dijktra on both the source and the destination. Then for find the vertices in the
        // graph for which the distance to vertex from source + distance to vertex from
        // destination is minimal.
        //
        // Conclusion: this does not consider all paths unfortunately. See test 5 (./print_test
        // 4 < testsets/test1.in).

        // IDEA 2
        //
        // Run Dijkstra on the destination (finish). From the source, add to the flow and
        // recurse on the edges that:
        //   - edge.dist + dijksta_dist[v] == dijkstra_dist[u]
        //
        // It would be better to do this in a DFS manner maybe?

        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);

        edge_desc e; int flag;
        for (size_t i = 0; i < m; i++)
        {
            // a : intersection
            // b : intersection
            // c : width (capacity)
            // d : length
            int a, b, c, d; std::cin >> a >> b >> c >> d;
            boost::tie(e, flag) = boost::edge(a, b, G);
            if (flag && d <= weights[e]) {
                if (d == weights[e])
                    cap[a][b] = cap[b][a] = cap[a][b] + c;
                else {
                    weights[e] = d;
                    cap[a][b] = cap[b][a] = c;
                }
            } else if (!flag) {
                e = boost::add_edge(a, b, G).first; weights[e] = d;
                cap[a][b] = cap[b][a] = c;
            }
        }

        std::vector<int> dist_map(n);
        dijkstra_dist(G, f, dist_map);

        graph FG(n);
        edge_adder adder(FG);

        // DFS

        std::stack<int> S;
        std::vector<int> visited(n, false);

        // Starting at source
        S.push(s);
        visited[s] = true;

        out_edge_it ebeg, eend;
        while (!S.empty()) {
            int u = S.top(); S.pop();
            boost::tie(ebeg, eend) = boost::out_edges(u, G);
            for (; ebeg != eend; ebeg++) {
                int v = boost::target(*ebeg, G);
                if (weights[*ebeg] + dist_map[v] == dist_map[u])
                    adder.add_edge(u, v, cap[u][v]);
                if (!visited[v]) {
                    S.push(v);
                    visited[v] = true;
                }
            }
        }

        std::cout << boost::push_relabel_max_flow(FG, s, f) << std::endl;

    }
    return 0;
}
