#include <iostream>
#include <vector>

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
        int Z, J; std::cin >> Z >> J;

        // ticket costs
        int c[Z];
        for (size_t i = 0; i < Z; ++i) std::cin >> c[i];

        // job rewards
        int p[J];
        for (size_t j = 0; j < J; ++j) std::cin >> p[j];

        // job zones
        std::vector<int> job_zones[J];
        for (size_t i = 0; i < J; ++i) {
            int N; std::cin >> N;
            for (size_t j = 0; j < N; j++) {
                int zone; std::cin >> zone;
                job_zones[i].push_back(zone);
            }
        }

        // IDEA 1
        //
        // Model the problem as a max-flow with the zones as the nodes.
        //
        // Each zone gets from the source a capacity of the zones profit, which is the sum of
        // all the zones that start at that zone minus the ticket for that zone.
        //
        // Then this zone passes on to each next zone the sum of the profits of jobs that need
        // to carry onto that zone. The jobs ending at that zone work toward the capacity of
        // the edge going to the destination.

        // Initialize with negative ticket costs
        std::vector<int> profits_from_source;
        for (size_t i = 0; i < Z; i++) {
            profits_from_source.push_back(-c[i]);
        }

        std::vector<int> profits_between[Z];
        for (size_t i = 0; i < Z; i++)
            for (size_t j = 0; j < Z; j++)
            {
                profits_between[i].push_back(-c[j]);
            }


        std::vector<int> profits_to_sink;
        for (size_t i = 0; i < Z; i++) {
            profits_to_sink.push_back(-c[i]);
        }

        int easy_money = 0;
        for (size_t i = 0; i < J; i++) {
            if (job_zones[i].empty()) {
                easy_money += p[i];
                continue;
            }
            int zone_start = job_zones[i].front();
            int zone_end   = job_zones[i].back();
            profits_from_source[zone_start] += p[i];
            profits_to_sink[zone_end] += p[i];
            for (size_t j = 0; j < job_zones[i].size() - 1; j++)
            {
                int zone_from = job_zones[i][j];
                int zone_to   = job_zones[i][j + 1];
                profits_between[zone_from][zone_to] += p[i];
            }
        }

        graph G(Z);
        edge_adder adder(G);

        const vertex_desc v_source = boost::add_vertex(G);
        const vertex_desc v_sink   = boost::add_vertex(G);

        adder.add_edge(v_source, v_sink, easy_money);

        for (size_t i = 0; i < Z; i++)
        {
            if (profits_from_source[i] > 0)
                adder.add_edge(v_source, i, profits_from_source[i]);
            if (profits_to_sink[i] > 0)
                adder.add_edge(i, v_sink, profits_to_sink[i]);
            for (size_t j = i + 1; j < Z; j++)
            {
                if (profits_between[i][j] > 0)
                    adder.add_edge(i, j, profits_between[i][j]);
            }
        }

        std::cout << boost::push_relabel_max_flow(G, v_source, v_sink) << std::endl;




    }
    return 0;
}
