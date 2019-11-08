#include <vector>
using namespace std;

class Graph {
    int num_vertices;
    vector<vector<int>> edges;

    public:
        Graph(int);

        void set_edge(int v1, int v2) {
            edges[v1].push_back(v2);
            edges[v2].push_back(v1);
        }

        // number of vertices getter
        int get_num_vertices() {
            return num_vertices;
        }

        // list of neighbors getter
        vector<int> get_neighbors(int v) {
            return edges[v];
        }
};

Graph::Graph(int p_num_vertices) {
    num_vertices = p_num_vertices;
    for (int v = 0; v < num_vertices; ++v) {
        vector<int> vect;
        edges.push_back(vect);
    }
}

enum Color { White, Gray, Black };

class DFSGraph {
    int time;
    Graph *graph;

    vector<Color> colors;
    vector<int> discovery;
    vector<int> finishing;

    public:
        DFSGraph(Graph *);
        void dfs_visit(int);

        vector<int> get_discovery() { return discovery; }
        vector<int> get_finishing() { return finishing; }
};

DFSGraph::DFSGraph(Graph *g) {
    time = 0;
    graph = g;
    for (int i = 0; i < g->get_num_vertices(); ++i) {
        colors.push_back(White);
        discovery.push_back(-1);
        finishing.push_back(-1);
    }
}

void DFSGraph::dfs_visit(int v) {
    discovery[v] = time++;
    colors[v] = Gray;

    vector<int> neighbors = graph->get_neighbors(v);

    vector<int>::iterator it = neighbors.begin();
    for (; it != neighbors.end(); it++) {
        int w = *it;
        if (colors[w] == White)
            dfs_visit(w);
    }

    colors[v] = Black;
    finishing[v] = time++;
}