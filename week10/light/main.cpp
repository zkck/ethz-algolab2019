#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_with_info_2<size_t, K>      Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Delaunay;
typedef Delaunay::Point                                             Point;

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        // number of participants, number of lamps
        int m, n; std::cin >> m >> n;

        std::vector<Point> participants;

        int radii[m];

        for (size_t i = 0; i < m; i++)
        {
            // position on stage and radius
            int x, y; std::cin >> x >> y;
            participants.push_back(Point(x, y));
            std::cin >> radii[i];
        }

        size_t h; std::cin >> h;

        std::vector<std::pair<Point, size_t>> lamps;

        for (size_t i = 0; i < n; i++)
        {
            int x, y; std::cin >> x >> y;
            lamps.push_back(std::make_pair(Point(x, y), i));
        }

        Delaunay T;
        T.insert(lamps.begin(), lamps.end());
        
        for (size_t i = 0; i < m; i++)
        {
            Point p = participants[i];
            Point l = T.nearest_vertex(p)->point();
            
            // first group of testsets:
            //   - at least one participant is still in the game after
            //     all lamps have been lit
            //
            // which means that the winners are those that have not been
            // hit

            if (CGAL::squared_distance(p, l) >= (radii[i] + h) * (radii[i] + h))
                std::cout << i << " ";
        }

        std::cout << std::endl;
        
        
    }
    
    return 0;
}
