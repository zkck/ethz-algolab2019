#include <iostream>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef int IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;


int main(int argc, char const *argv[])
{
    int n, m; std::cin >> n >> m;

    while (!(n == 0 && m == 0))
    {
        std::pair<int, int> daily_amounts[n];

        for (int i = 0; i < n; ++i)
        {
            // minimal and maximal daily amount of i-th nutrient
            int min, max; std::cin >> min >> max;
            daily_amounts[i] = std::pair<int, int>(min, max);
        }

        int nutrients[m][n];
        int prices[m];
        for (int j = 0; j < m; ++j) {
            std::cin >> prices[j];
            for (int i = 0; i < n; ++i) {
                std::cin >> nutrients[j][i];
            }
        }

        Program lp (CGAL::SMALLER, true, 0, false, 0);

        // we want to minimize the food coefficients

        // objective function
        for (int j = 0; j < m; ++j)
        {
            lp.set_c(j, prices[j]);
        }

        int id = 0;

        for (int i = 0; i < n; ++i)
        {
            for (int j = 0; j < m; ++j)
            {
                lp.set_a(j, id, nutrients[j][i]);
            }
            lp.set_b(id++, daily_amounts[i].second);
        }

        for (int i = 0; i < n; ++i)
        {
            for (size_t j = 0; j < m; j++)
            {
                lp.set_a(j, id, -nutrients[j][i]);
            }
            lp.set_b(id++, -daily_amounts[i].first);
        }

        Solution s = CGAL::solve_linear_program(lp, ET());

        if (s.is_optimal()) {
            double cost = CGAL::to_double(s.objective_value());
            std::cout << std::floor(cost) << std::endl;
        } else {
            std::cout << "No such diet." << std::endl;
        }
        std::cin >> n >> m;
    }
    return 0;
}
