#include <iostream>
#include <string>

#define NUM_CHARS 26

// BGL include
#include <boost/graph/adjacency_list.hpp>

// BGL flow include *NEW*
#include <boost/graph/push_relabel_max_flow.hpp>

// Graph Type with nested interior edge properties for flow algorithms
typedef boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS> traits;
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS, boost::no_property,
                              boost::property<boost::edge_capacity_t, long,
                                              boost::property<boost::edge_residual_capacity_t, long,
                                                              boost::property<boost::edge_reverse_t, traits::edge_descriptor>>>>
    graph;

typedef traits::vertex_descriptor vertex_desc;
typedef traits::edge_descriptor edge_desc;

// Custom edge adder class, highly recommended
class edge_adder
{
    graph &G;

public:
    explicit edge_adder(graph &G) : G(G) {}

    void add_edge(int from, int to, long capacity)
    {
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

int h, w;

std::pair<int, int> back(int i, int j)
{
    return std::pair<int, int>(i, w - j - 1);
}

int index(char c)
{
    return (int)c - (int)'A';
}

char to_char(int index)
{
    return (char)((int)'A' + index);
}

int vertex(int i, int j)
{
    return j * w + i;
}

int main(int argc, char const *argv[])
{
    int num_tests;
    std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        std::cin >> h >> w;

        // std::cout << h << " " << w << std::endl;

        // note from crime scene
        std::string note;
        std::cin >> note;

        // std::cout << note << std::endl;

        // front side
        char fs[h][w];
        for (int i = 0; i < h; ++i)
        {
            for (int j = 0; j < w; ++j)
            {
                std::cin >> fs[i][j];
                // std::cout << fs[i][j];
            }
            // std::cout << std::endl;
        }

        // back side
        char bs[h][w];
        for (int i = 0; i < h; ++i)
        {
            for (int j = 0; j < w; ++j)
            {
                std::cin >> bs[i][j];
                // std::cout << bs[i][j];
            }
            // std::cout << std::endl;
        }

        char counts[NUM_CHARS] = {0};
        for (int i = 0; i < note.length(); ++i)
        {
            counts[index(note[i])]++;
        }

        // build graph

        graph G(1 + NUM_CHARS + h * w + 1);
        edge_adder adder(G);

        // common vertices
        int source = 0;
        int sink = 1 + NUM_CHARS + h * w;

        for (int c = 0; c < NUM_CHARS; ++c)
        {
            adder.add_edge(source, 1 + c, counts[c]);

            // find all valid letters that have either back or front
            for (int index = 0; index < h * w; index++)
            {
                int i = index / w;
                int j = index % w;
                int bi, bj;
                std::tie(bi, bj) = back(i, j);
                if (fs[i][j] == to_char(c) || bs[bi][bj] == to_char(c)) {
                    adder.add_edge(1 + c, 1 + NUM_CHARS + index, 1);
                }
            }
        }

        for (int index = 0; index < h * w; ++index)
        {
            adder.add_edge(1 + NUM_CHARS + index, sink, 1);
        }

        int max_flow = boost::push_relabel_max_flow(G, source, sink);

        if (max_flow == note.length())
            std::cout << "Yes" << std::endl;
        // else if (max_flow < note.length())
        //     std::cout << "No" << std::endl;
        else
            std::cout << "No" << std::endl;

        // delete &G;
    }
    return 0;
}
