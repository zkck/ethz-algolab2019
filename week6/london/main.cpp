#include <iostream>
#include <string>

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

        int h, w; std::cin >> h >> w;

        std::string note; std::cin >> note;

        char front[h][w], back[h][w];

        for (size_t i = 0; i < h; i++)
        {
            for (size_t j = 0; j < w; j++)
            {
                std::cin >> front[i][j];
            }
        }

        for (size_t i = 0; i < h; i++)
        {
            for (size_t j = 0; j < w; j++)
            {
                std::cin >> back[i][j];
            }
        }

        graph G(2 + h * w + note.length());
        edge_adder adder(G);

        for (size_t idx = 0; idx < h * w; idx++)
        {
            adder.add_edge(0, 1 + idx, 1);

            int i = idx / w;
            int j = idx % w;

            int back_i = i;
            int back_j = w - j - 1;

            for (size_t c = 0; c < note.length(); c++)
            {
                if (note[c] == back[back_i][back_j] || note[c] == front[i][j])
                    adder.add_edge(1 + idx, 1 + (h * w) + c, 1);                
            }
        }

        for (size_t i = 0; i < note.length(); i++)
        {
            adder.add_edge(1 + h * w + i, 1 + h * w + note.length(), 1);
        }

        int flow = boost::push_relabel_max_flow(G, 0, 1 + h * w + note.length());

        if (flow == note.length())
            std::cout << "Yes" << std::endl;
        else
            std::cout << "No" << std::endl;
        
        
            
    }
    return 0;
}
