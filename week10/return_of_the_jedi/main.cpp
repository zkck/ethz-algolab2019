#include <iostream>
#include <limits>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS,
                              boost::no_property, boost::property<boost::edge_weight_t, int>>
    weighted_graph;
typedef boost::property_map<weighted_graph, boost::edge_weight_t>::type weight_map;
typedef boost::graph_traits<weighted_graph>::edge_descriptor edge_desc;
typedef boost::graph_traits<weighted_graph>::vertex_descriptor vertex_desc;

void algo(weighted_graph &G)
{
    std::vector<edge_desc> mst; // vector to store MST edges (not a property map!)

    boost::kruskal_minimum_spanning_tree(G, std::back_inserter(mst));
    weight_map weights = boost::get(boost::edge_weight, G);

    int best = -1;

    for (std::vector<edge_desc>::iterator it = mst.begin(); it != mst.end(); ++it)
    {
        std::vector<edge_desc> removed_mst;
        int saved = weights[*it];
        weights[*it] = std::numeric_limits<int>::max();
        boost::kruskal_minimum_spanning_tree(G, std::back_inserter(removed_mst));
        int sum = 0;
        for (edge_desc &e : removed_mst)
        {
            // std::cout << "weight of " << e << " is " << weights[e] << std::endl;
            sum += weights[e];
        }
        if (best == -1 || sum < best)
            best = sum;
        weights[*it] = saved;
    }

    std::cout << best << std::endl;
}

int main(int argc, char const *argv[])
{
    int num_tests;
    std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        // std::cout << "new test" << std::endl;
        int n, i;
        std::cin >> n >> i;

        // printf("n=%d, i=%d\n", n, i);

        weighted_graph G(n);
        weight_map weights = boost::get(boost::edge_weight, G);

        for (size_t j = 1; j <= n - 1; j++)
        {
            for (size_t k = 1; k <= n - j; k++)
            {
                // printf("adding edge from %ld to %ld...\n", j, j + k);
                int w;
                std::cin >> w;
                edge_desc e = boost::add_edge(j - 1, j + k - 1, G).first;
                weights[e] = w;
            }
        }

        algo(G);
    }
    return 0;
}
