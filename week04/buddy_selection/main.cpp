#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// BGL includes
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::undirectedS> graph;
typedef boost::graph_traits<graph>::vertex_descriptor                       vertex_desc;

int maximum_matching(const graph &g) {
    int n = boost::num_vertices(g);

    std::vector<vertex_desc> mate_map(n);

    boost::edmonds_maximum_cardinality_matching(g,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, g)));
    int matching_size = boost::matching_size(g,
        boost::make_iterator_property_map(mate_map.begin(), boost::get(boost::vertex_index, g)));

    return matching_size;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int n, c, f; std::cin >> n >> c >> f;

        std::vector<std::string> keywords[n];

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < c; ++j) {
                std::string kw; std::cin >> kw;
                keywords[i].push_back(kw);
            }
            std::sort(keywords[i].begin(), keywords[i].end());
        }

        graph buddy_graph(n);

        for (int i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                std::vector<std::string> intersection(c);
                auto it = std::set_intersection(
                    keywords[i].begin(),
                    keywords[i].end(),
                    keywords[j].begin(),
                    keywords[j].end(),
                    intersection.begin());
                intersection.resize(it - intersection.begin());
                if (intersection.size() > f)
                    boost::add_edge(i, j, buddy_graph);
            }
        }

        if (maximum_matching(buddy_graph) * 2 == n)
            std::cout << "not optimal" << std::endl;
        else
            std::cout << "optimal" << std::endl;

    }
    return 0;
}
