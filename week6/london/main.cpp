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

int to_index(char c) {
    assert((int) 'A' <= (int) c && (int) c <= (int) 'Z');
    return (int) c - (int) 'A'; 
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        
        // height and width of the paper
        int h, w; std::cin >> h >> w;

        // note from the crime scene
        std::string note; std::cin >> note;

        size_t note_counts[NUM_CHARS] = { 0 };
        for (char c: note)
            note_counts[to_index(c)]++;

        // characters of front and back side

        std::string front_side[h], back_side[h];

        for (size_t i = 0; i < h; i++)
            std::cin >> front_side[i];

        for (size_t i = 0; i < h; i++)
            std::cin >> back_side[i];
            
        // counts of each character on front and back side

        size_t front_counts[NUM_CHARS] = { 0 }, back_counts[NUM_CHARS] = { 0 };

        for (size_t i = 0; i < h; i++)
            for (size_t j = 0; j < w; j++)
                front_counts[to_index(front_side[i][j])]++;
        
        for (size_t i = 0; i < h; i++)
            for (size_t j = 0; j < w; j++)
                back_counts[to_index(back_side[i][j])]++;

        // adjacency matrix counts of edges to and from

        size_t conn_counts[NUM_CHARS][NUM_CHARS] = { 0 };

        for (size_t i = 0; i < h; i++)
            for (size_t j = 0; j < w; j++) {
                size_t back_i = i;
                size_t back_j = w - j - 1;
                char f = front_side[i][j];
                char b = back_side[back_i][back_j];
                conn_counts[to_index(f)][to_index(b)]++;
            }

        // hope this works

        graph G(1 + 3 * NUM_CHARS + 1);
        edge_adder adder(G);

        size_t source = 0;
        size_t sink = 1 + 3 * NUM_CHARS;

        // layer 0-1, 1-2, 1-3
        for (size_t i = 0; i < NUM_CHARS; i++)
        {
            if (note_counts[i] > 0)
                adder.add_edge(source, 1 + i, note_counts[i]);

            // connect to back and front
            if (front_counts[i] > 0)
                adder.add_edge(1 + i, 1 +     NUM_CHARS + i, front_counts[i]);

            if (back_counts[i] > 0)
                adder.add_edge(1 + i, 1 + 2 * NUM_CHARS + i, back_counts[i]);
        }

        // layer 2-3
        for (size_t i = 0; i < NUM_CHARS; i++)
            for (size_t j = 0; j < NUM_CHARS; j++) {
                size_t l1 = 1 +     NUM_CHARS + i;
                size_t l2 = 1 + 2 * NUM_CHARS + j;

                size_t count = conn_counts[i][j];
                if (count > 0)
                    adder.add_edge(l1, l2, count);
            }

        // layer 3-4
        for (size_t i = 0; i < NUM_CHARS; i++)
        {
            if (back_counts[i] > 0)
                adder.add_edge(1 + 2 * NUM_CHARS + i, sink, back_counts[i]);
        }

        long flow = boost::push_relabel_max_flow(G, source, sink);

        if (flow == note.length()) {
            std::cout << "Yes" << std::endl;
        } else {
            std::cout << "No" << std::endl;
        }
    }
    return 0;
}
