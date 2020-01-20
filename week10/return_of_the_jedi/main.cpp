#include <iostream>
#include <limits>
#include <stack>
#include <set>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;
typedef boost::graph_traits<weighted_graph>::out_edge_iterator out_edge_it;

int main(int argc, char const *argv[])
{
    int num_tests;
    std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n, i;
        std::cin >> n >> i;

        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);

        int cost; edge_desc e;
        for (size_t j = 1; j <= n - 1; j++)
        {
            for (size_t k = 1; k <= n - j; k++)
            {
                e = boost::add_edge(j - 1, j + k - 1, G).first;
                std::cin >> cost; weights[e] = cost;
            }
        }

        std::vector<edge_desc> mst;    // vector to store MST edges (not a property map!)
        boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));

        int mst_weight = 0;

        std::vector<vertex_desc> adj[n];
        for (edge_desc &edge : mst)
        {
            vertex_desc target = boost::target(edge, G);
            vertex_desc source = boost::source(edge, G);
            adj[target].push_back(source);
            adj[source].push_back(target);
            mst_weight += weights[edge];
        }

        int min_diff = std::numeric_limits<int>::max();

        for (edge_desc &edge : mst) {
            vertex_desc target = boost::target(edge, G);
            vertex_desc source = boost::source(edge, G);
            int weight = weights[edge];

            std::vector<vertex_desc> S;
            std::vector<int> visited(n, false);

            std::set<vertex_desc> source_set;

            S.push_back(source);
            visited[source] = true;
            visited[target] = true;
            while (!S.empty()) {
                const vertex_desc u = S.back(); S.pop_back();
                for (vertex_desc &v : adj[u]) {
                    if (!visited[v]) {
                        source_set.insert(v);
                        S.push_back(v);
                        visited[v] = true;
                    }
                }
            }

            S.clear();
            visited.clear();
            visited.resize(n, false);

            std::set<vertex_desc> target_set;

            S.push_back(target);
            visited[source] = true;
            visited[target] = true;
            while (!S.empty()) {
                const vertex_desc u = S.back(); S.pop_back();
                for (vertex_desc &v : adj[u]) {
                    if (!visited[v]) {
                        target_set.insert(v);
                        S.push_back(v);
                        visited[v] = true;
                    }
                }
            }

            // // debug
            // printf("  source=%ld target=%ld\n", source, target);
            // std::cout << "  source_set: ";
            // for (auto u : source_set)
            // {
            //     std::cout << u << " ";
            // }
            // std::cout << std::endl;
            // std::cout << "  target_set: ";
            // for (auto u : target_set)
            // {
            //     std::cout << u << " ";
            // }
            // std::cout << std::endl;

            int min_weight = std::numeric_limits<int>::max();
            out_edge_it ebeg, eend;

            // source
            boost::tie(ebeg, eend) = boost::out_edges(source, G);
            for (; ebeg != eend; ebeg++) {
                if (target_set.find(boost::target(*ebeg, G)) != target_set.end())
                    min_weight = std::min(min_weight, weights[*ebeg]);
            }

            // target
            boost::tie(ebeg, eend) = boost::out_edges(target, G);
            for (; ebeg != eend; ebeg++) {
                if (source_set.find(boost::target(*ebeg, G)) != source_set.end())
                    min_weight = std::min(min_weight, weights[*ebeg]);
            }

            assert(min_weight != std::numeric_limits<int>::max());

            int diff = min_weight - weight;
            assert(diff >= 0);

            min_diff = std::min(min_diff, diff);
        }

        std::cout << mst_weight + min_diff << std::endl;


    }
    return 0;
}
