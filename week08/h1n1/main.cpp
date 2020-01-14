#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <vector>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

// Incremental CCs
#include <boost/foreach.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/incremental_components.hpp>
#include <boost/pending/disjoint_sets.hpp>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K>                Vb;
typedef CGAL::Triangulation_face_base_with_info_2<size_t,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>         Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>               Delaunay;
typedef Delaunay::Point                                     Point;

// Incremental CCs

typedef boost::adjacency_list <boost::vecS, boost::vecS, boost::undirectedS>    Graph;
typedef boost::graph_traits<Graph>::vertex_descriptor                           Vertex;
typedef boost::graph_traits<Graph>::vertices_size_type                          VertexIndex;

typedef VertexIndex*    Rank;
typedef Vertex*         Parent;

typedef Delaunay::Face_iterator Fh;

// Helper types
struct mission {
    size_t index;
    long d;
    int x;
    int y;
};
struct edge {
    long sq_dist;
    size_t from_face;
    size_t to_face;
};

// Comparators
int by_distance(struct mission &m1, struct mission &m2) {
    return m1.d > m2.d;
}


double intersection_length(Fh &face, Fh &neighbor) {
    std::vector<size_t> joining;
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if (face->vertex(i) == neighbor->vertex(j))
                joining.push_back(i);
        }
    }
    assert (joining.size() == 2);
    double dist = CGAL::squared_distance(
        face->vertex(joining[0])->point(),
        face->vertex(joining[1])->point()
    );
    return dist;
}

int algo(int n)
{
    std::vector<Point> points;
    for (size_t i = 0; i < n; i++)
    {
        int x, y; std::cin >> x >> y;
        points.push_back(Point(x, y));
    }

    Delaunay T;
    T.insert(points.begin(), points.end());

    int m; std::cin >> m;

    std::vector<struct mission> missions;

    for (size_t i = 0; i < m; i++)
    {
        int x, y; std::cin >> x >> y;
        long d; std::cin >> d;
        struct mission mission = { i, d, x, y };
        missions.push_back(mission);
    }

    // STEP 1: SORT MISSIONS WITH MOST DEMANDING FIRST

    std::sort(missions.begin(), missions.end(), by_distance);

    // STEP 2: PREPARE THE INCREMENTAL CONNECTED COMPONENTS GRAPH

    // Give an index to all vertices
    size_t num_faces = 0;
    for (auto fc = T.all_faces_begin(); fc != T.all_faces_end(); ++fc)
        fc->info() = num_faces++;

    // CREATE THE GRAPH

    Graph graph(num_faces);

    std::vector<VertexIndex> rank(num_vertices(graph));
    std::vector<Vertex> parent(num_vertices(graph));

    boost::disjoint_sets<Rank, Parent> ds(&rank[0], &parent[0]);

    boost::initialize_incremental_components(graph, ds);
    boost::incremental_components(graph, ds);

    boost::graph_traits<Graph>::edge_descriptor edge;
    bool flag;

    // Connect all infinite faces to the same CC
    Delaunay::Face_circulator fc = T.incident_faces(T.infinite_vertex());
    size_t base = fc->info();
    while (++fc != T.incident_faces(T.infinite_vertex())) {
        boost::tie(edge, flag) = boost::add_edge(fc->info(), base, graph);
        ds.union_set(fc->info(), base);
    }

    // Collect all the edges
    std::vector<struct edge> edges;
    Delaunay::Finite_faces_iterator fit = T.finite_faces_begin();
    for (; fit != T.finite_faces_end(); fit++) {
        size_t from = fit->info();
        for (size_t i = 0; i < 3; i++) {
            size_t to = fit->neighbor(i)->info();
            long sq_dist = intersection_length(fit, fit->neighbor(i));
        }

    }







    return 0;
}
int main()
{
  int n;
  for (std::cin >> n; n > 0; std::cin >> n) algo(n);
  return 0;
}
