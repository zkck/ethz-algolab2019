#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

#include <vector>
#include <queue>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Triangulation_vertex_base_2<K>                Vb;
typedef CGAL::Triangulation_face_base_with_info_2<size_t,K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb>         Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>               Delaunay;
typedef Delaunay::Point                                     Point;

// BFS
typedef Delaunay::Face_handle   Fh;
typedef Delaunay::Vertex_handle Vh;

double intersection_length(Fh &from, Fh &to) {
    std::vector<size_t> joining;
    for (size_t i = 0; i < 3; i++) {
        for (size_t j = 0; j < 3; j++) {
            if (from->vertex(i) == to->vertex(j))
                joining.push_back(i);
        }
    }
    assert (joining.size() == 2);
    double dist = CGAL::squared_distance(
        from->vertex(joining[0])->point(),
        from->vertex(joining[1])->point()
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

    for (size_t i = 0; i < m; i++)
    {
        int x, y; std::cin >> x >> y;
        Point user(x, y);

        long d; std::cin >> d;

        // First check if user is already infected
        Delaunay::Vertex_handle closest_infected = T.nearest_vertex(user);
        if (CGAL::squared_distance(user, closest_infected->point()) < d) {
            std::cout << "n";
            continue;
        }

        // BFS

        // Give an index to all vertices
        size_t num_faces = 0;
        for (auto fc = T.all_faces_begin(); fc != T.all_faces_end(); ++fc)
            fc->info() = num_faces++;

        std::queue<Fh> Q;
        std::vector<int> visited(num_faces, false);

        // Locate the face on which the user is standing
        Fh src = T.locate(user);
        Q.push(src);
        visited[src->info()] = true;

        int escaped = false;
        while (!Q.empty()) {
            Fh v = Q.front(); Q.pop();

            if (T.is_infinite(v)) {
                escaped = true;
                break;
            }

            for (size_t i = 0; i < 3; i++)
            {
                Fh u = v->neighbor(i);
                if (intersection_length(v, u) >= 4 * d && !visited[u->info()]) {
                    visited[u->info()] = true;
                    Q.push(u);
                }
            }
        }

        if (escaped)
            std::cout << "y";
        else
            std::cout << "n";
    }

    std::cout << std::endl;


    return 0;
}
int main()
{
  int n;
  for (std::cin >> n; n > 0; std::cin >> n) algo(n);
  return 0;
}
