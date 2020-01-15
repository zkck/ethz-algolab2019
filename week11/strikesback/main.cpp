#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
#include <iostream>
#include <iomanip>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>                   Delaunay;

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpzf.h>

// choose input type (input coefficients must fit)
typedef double IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpzf ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


struct asteroid {
    int x;
    int y;
    int d;
};

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        // a : number of asteroids to be destroyed
        // s : number of laster shots
        // b : number of bounty hunters
        int a, s, b; std::cin >> a >> s >> b;
        int e; std::cin >> e;

        // IDEA 1
        //
        // - Build a triangulation with the bounty hunters
        // - For each shot, find the nearest bounty hunter -> this is the radius of the shot
        // - Build a linear program
        //   - For each asteroid, build equation that fulfills asteroid's density
        //   - Sum of energies must be <= to total energy

        std::vector<struct asteroid> asteroids;
        for (size_t i = 0; i < a; i++)
        {
            int x, y, d; std::cin >> x >> y >> d;
            struct asteroid asteroid = { x, y, d };
            asteroids.push_back(asteroid);
        }

        std::vector<K::Point_2> shots;
        for (size_t i = 0; i < s; i++)
        {
            int x, y; std::cin >> x >> y;
            shots.push_back(K::Point_2(x, y));
        }

        std::vector<K::Point_2> bounty_hunters;
        for (size_t i = 0; i < b; i++)
        {
            int x, y; std::cin >> x >> y;
            bounty_hunters.push_back(K::Point_2(x, y));
        }

        Delaunay T;
        T.insert(bounty_hunters.begin(), bounty_hunters.end());

        // squared radii of each shot
        std::vector<double> shot_radii;
        for (K::Point_2 shot : shots)
        {
            if (b == 0)
                shot_radii.push_back(std::numeric_limits<double>::infinity());
            else {
                auto v = T.nearest_vertex(shot);
                shot_radii.push_back(CGAL::squared_distance(v->point(), shot));
            }
        }

        assert(shot_radii.size() == shots.size());

        // Ax >= b <=> Ax >= density
        Program lp (CGAL::LARGER, true, 0, false, 0);

        // Debug
        std::cout << std::fixed << std::setprecision(5);

        for (size_t i = 0; i < a; i++) {
            struct asteroid asteroid = asteroids[i];
            K::Point_2 p(asteroid.x, asteroid.y);
            for (size_t j = 0; j < s; j++) {
                double sq_dist = CGAL::squared_distance(p, shots[j]);
                if (sq_dist > shot_radii[j]) continue;
                lp.set_a(j, i, 1 / std::max(1.0, sq_dist));
                // printf("e%ld * %.5f ", i, 1 / std::max(1.0, sq_dist));
            }
            // printf("<= %d\n", asteroids[i].d);
            lp.set_b(i, asteroids[i].d);
        }

        for (size_t j = 0; j < s; j++)
        {
            lp.set_c(j, 1);
        }

        Solution solution = CGAL::solve_linear_program(lp, ET());
        if (solution.is_optimal()) {
            auto energy = solution.objective_value();
            if (energy <= e)
                std::cout << "y";
            else
                std::cout << "n";
        } else {
            std::cout << "n";
        }
        std::cout << std::endl;








    }
    return 0;
}
