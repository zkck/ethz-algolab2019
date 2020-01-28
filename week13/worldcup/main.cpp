#include <iostream>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef CGAL::Gmpq IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <vector>
typedef CGAL::Exact_predicates_inexact_constructions_kernel         K;
typedef CGAL::Triangulation_vertex_base_2<K>                        Vb;
typedef CGAL::Triangulation_data_structure_2<Vb>                    Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>                      Delaunay;
typedef Delaunay::Point                                             Point;


struct warehouse {
    int x;
    int y;
    IT s;
    IT a;
};

struct stadium {
    int x;
    int y;
    IT d;
    IT u;
};

int n, m, c;

double floor_to_double(const ET x) {
  double a = std::floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a+1 <= x) a += 1;
  return a;
}


int quantity(int wh, int st) {
    return wh * m + st;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        // n : number of warehouses
        // m : number of stadiums
        // c : number of contour lines
        std::cin >> n >> m >> c;

        std::vector<struct warehouse> warehouses;
        for (size_t i = 0; i < n; i++)
        {
            struct warehouse warehouse;
            // x : position
            // y : position
            // s : supply in liters
            // a : percentage of alcohol
            std::cin >> warehouse.x >> warehouse.y >> warehouse.s >> warehouse.a;
            warehouses.push_back(warehouse);
        }

        std::vector<struct stadium> stadiums;
        for (size_t i = 0; i < m; i++)
        {
            struct stadium stadium;
            // x : position
            // y : position
            // d : demand in liters
            // u : upper bound for liters of alcohol
            std::cin >> stadium.x >> stadium.y >> stadium.d >> stadium.u;
            stadiums.push_back(stadium);
        }


        IT r[n][m];
        for (size_t i = 0; i < n; i++) {
            for (size_t j = 0; j < m; j++) {
                // r[i][j] : revenue for moving a liter from warehouse i to stadium j
                std::cin >> r[i][j];
            }
        }

        // Fourth group of testsets states that at most 100 contour lines contain at least one
        // warehouse in their interior.
        //
        // This suggest triangulating the stadiums/warehouses, and checking if the contour
        // line is in range of any stadiums or warehouses.

        std::vector<Point> points;
        for (auto &stadium : stadiums)
            points.push_back(Point(stadium.x, stadium.y));
        for (auto &warehouse : warehouses)
            points.push_back(Point(warehouse.x, warehouse.y));

        Delaunay T;
        T.insert(points.begin(), points.end());

        // Keep track of conflicting contour lines on stadiums and on warehouses
        std::vector<size_t> st_conflicts[m];
        std::vector<size_t> wh_conflicts[n];

        for (size_t i = 0; i < c; i++)
        {
            // x : position
            // y : position
            // r : radius
            int x, y, r; std::cin >> x >> y >> r;

            Point p(x, y);
            auto v = T.nearest_vertex(p);

            if (CGAL::squared_distance(p, v->point()) > r * r)
                continue;

            // Check stadiums conflicts
            for (size_t j = 0; j < m; ++j) {
                auto st = stadiums[j];
                if (CGAL::squared_distance(Point(st.x, st.y), p) < r * r) {
                    st_conflicts[j].push_back(i);
                }
            }

            // Check warehouse conflicts
            for (size_t j = 0; j < n; ++j) {
                auto wh = warehouses[j];
                if (CGAL::squared_distance(Point(wh.x, wh.y), p) < r * r) {
                    wh_conflicts[j].push_back(i);
                }
            }
        }

        Program lp (CGAL::SMALLER, true, 0, false, 0);
        int num_rows = 0;

        // STEP 1
        //
        // For each stadium, set alcohol from all warehouses times their alcohol percentage
        // to less than the upper bound.

        for (size_t st = 0; st < m; st++) {
            for (size_t wh = 0; wh < n; wh++) {
                lp.set_a(quantity(wh, st), st, warehouses[wh].a / 100.0);
            }
            lp.set_b(st, stadiums[st].u);
        }

        num_rows += m;

        // STEP 2
        //
        // For each warehouse, limit the sum of the supply to all stadiums to that warehouse's
        // supply.

        for (size_t wh = 0; wh < n; wh++) {
            for (size_t st = 0; st < m; st++) {
                lp.set_a(quantity(wh, st), num_rows + wh, 1);
            }
            lp.set_b(num_rows + wh, warehouses[wh].s);
        }

        num_rows += n;

        // STEP 3
        //
        // The sum of quantities to a stadium must be equal to the demand.

        for (size_t st = 0; st < m; st++)
        {
            int upper = st * 2;
            int lower = st * 2 + 1;
            for (size_t wh = 0; wh < n; wh++)
            {
                lp.set_a(quantity(wh, st), num_rows + upper,  1);
                lp.set_a(quantity(wh, st), num_rows + lower, -1);
            }
            lp.set_b(num_rows + upper,  stadiums[st].d);
            lp.set_b(num_rows + lower, -stadiums[st].d);
        }

        num_rows += 2 * m;

        // FINAL STEP (objective function)
        //
        // Multiply each dispatched quantity with the revenue (must be negated since LP
        // minimizes).

        for (size_t wh = 0; wh < n; wh++) {
            for (size_t st = 0; st < m; st++) {
                std::vector<size_t> conflicts;
                std::set_union(
                    st_conflicts[st].begin(), st_conflicts[st].end(),
                    wh_conflicts[wh].begin(), wh_conflicts[wh].end(),
                    std::back_inserter(conflicts));
                lp.set_c(quantity(wh, st), -r[wh][st] + (conflicts.size() / 100.0));
            }
        }

        // SOLUTION

        Solution s = CGAL::solve_linear_program(lp, ET());
        if (s.is_optimal()) {
            std::cout << (int) std::floor(-CGAL::to_double(s.objective_value())) << std::endl;
        } else if (s.is_unbounded()) {
            std::cout << "nope" << std::endl;
        } else if (s.is_infeasible()) {
            std::cout << "RIOT!" << std::endl;
        }
    }
    return 0;
}
