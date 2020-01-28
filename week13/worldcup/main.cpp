#include <iostream>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef double IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

struct warehouse {
    int x;
    int y;
    int s;
    int a;
};

struct stadium {
    int x;
    int y;
    int d;
    int u;
};

int var(int wh, int st, int m) {
    return wh * m + st;
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int n, m, c; std::cin >> n >> m >> c;

        std::vector<struct warehouse> warehouses;
        for (size_t i = 0; i < n; i++)
        {
            struct warehouse warehouse;
            std::cin >> warehouse.x >> warehouse.y >> warehouse.s >> warehouse.a;
            warehouses.push_back(warehouse);
        }
        std::vector<struct stadium> stadiums;
        for (size_t i = 0; i < m; i++)
        {
            struct stadium stadium;
            std::cin >> stadium.x >> stadium.y >> stadium.d >> stadium.u;
            stadiums.push_back(stadium);
        }
        int revenues[n][m];
        for (size_t i = 0; i < n; i++)
        {
            for (size_t j = 0; j < m; j++)
            {
                std::cin >> revenues[i][j];
            }
        }
        for (size_t i = 0; i < c; i++)
        {
            int x, y, r; std::cin >> x >> y >> r;
        }

        Program lp (CGAL::SMALLER, true, 0, false, 0);
        int num_rows = 0;

        // for each stadium, set alcohol's upper limit
        for (size_t st = 0; st < m; st++) {
            for (size_t wh = 0; wh < n; wh++) {
                lp.set_a(var(wh, st, m), st, warehouses[wh].a / 100.0);
            }
            lp.set_b(st, stadiums[st].u);
        }

        num_rows += m;

        // for each warehouse, limit the supply to all stadiums
        for (size_t wh = 0; wh < n; wh++)
        {
            for (size_t st = 0; st < m; st++)
            {
                lp.set_a(var(wh, st, m), num_rows + wh, 1);
            }
            lp.set_b(num_rows + wh, warehouses[wh].s);
        }

        num_rows += n;

        // upper bound and lower bound on the demand of the stadiums
        for (size_t st = 0; st < m; st++)
        {
            int upper = st * 2;
            int lower = st * 2 + 1;
            for (size_t wh = 0; wh < n; wh++)
            {
                lp.set_a(var(wh, st, m), num_rows + upper,  1);
                lp.set_a(var(wh, st, m), num_rows + lower, -1);
            }
            lp.set_b(num_rows + upper,  stadiums[st].d);
            lp.set_b(num_rows + lower, -stadiums[st].d);
        }

        num_rows += 2 * m;

        for (size_t wh = 0; wh < n; wh++)
        {
            for (size_t st = 0; st < m; st++)
            {
                lp.set_c(var(wh, st, m), -revenues[wh][st]);
            }
        }




        Solution s = CGAL::solve_linear_program(lp, ET());
        if (s.is_optimal()) {
            std::cout << s.objective_value() << std::endl;
        } else if (s.is_unbounded()) {
            std::cout << "nope" << std::endl;
        } else if (s.is_infeasible()) {
            std::cout << "RIOT!" << std::endl;
        }











    }
    return 0;
}
