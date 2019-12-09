#include <iostream>
#include <vector>
#include <queue>

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

        std::vector<size_t> winners;
        size_t winning_round = 0; // did not work with int -1
        
        for (size_t i = 0; i < m; i++)
        {
            Point p = participants[i];
            Delaunay::Vertex_handle l = T.nearest_vertex(p);

            // radius in which a lamp must be to light up the participant
            size_t limit = (radii[i] + h) * (radii[i] + h);
            
            // first group of testsets:
            //   - at least one participant is still in the game after
            //     all lamps have been lit
            //
            // which means that the winners are those that have not been
            // hit

            // if (CGAL::squared_distance(p, l->point()) >= limit)
            //     std::cout << i << " ";

            // second group of testsets:
            //   - no info
            //
            // try bfs from nearest vertex?

            // we are minimizing the eliminating round
            size_t eliminating_round = n;

            std::vector<bool> visited(n, false);
            std::queue<Delaunay::Vertex_handle> Q;

            if (CGAL::squared_distance(p, l->point()) < limit) {
                visited[l->info()] = true;
                Q.push(l);
            }

            while (!Q.empty())
            {
                const Delaunay::Vertex_handle u = Q.front();
                Q.pop();

                // check if eliminating round
                if (u->info() < eliminating_round) {
                    eliminating_round = u->info();
                    if (eliminating_round < winning_round)
                        break;
                }

                Delaunay::Vertex_circulator c = T.incident_vertices(u);
                do
                {
                    if (T.is_infinite(c)) continue;
                    bool in_limit = CGAL::squared_distance(p, c->point()) < limit;
                    if (in_limit && !visited[c->info()])
                    {
                        visited[c->info()] = true;
                        Q.push(c);
                    }
                } while (++c != T.incident_vertices(u));
            }

            if (eliminating_round >= winning_round) {
                if (eliminating_round > winning_round) {
                    winning_round = eliminating_round;
                    winners.clear();
                }
                winners.push_back(i);
            }
        }

        for (size_t w : winners)
            std::cout << w << " ";
        std::cout << std::endl;
        
        
    }
    
    return 0;
}
