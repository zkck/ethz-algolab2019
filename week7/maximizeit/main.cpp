#include <iostream>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpz.h>

// choose input type (input coefficients must fit)
typedef int IT;

// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpz ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

enum Var { X, Y, Z };

int main(int argc, char const *argv[])
{
    int p; std::cin >> p;

    while (p > 0)
    {
        int a, b; std::cin >> a >> b;

        if (p == 1)
        {
            Program lp (CGAL::SMALLER, true, 0, false, 0);

            int i = 0;

            // x + y <= 4
            lp.set_a(X, i, 1);  lp.set_a(Y, i, 1); lp.set_b(i++, 4);

            // 4x + 2y <= ab
            lp.set_a(X, i, 4);  lp.set_a(Y, i, 2); lp.set_b(i++, a * b);

            // -x + y <= 1
            lp.set_a(X, i, -1); lp.set_a(Y, i, 1); lp.set_b(i++, 1);

            // objective function
            // maximize -ax + by => minimize ax - by
            lp.set_c(X, a); lp.set_c(Y, -b);

            Solution s = CGAL::solve_linear_program(lp, ET());

            if (s.is_optimal())
            {
                double result = -CGAL::to_double(s.objective_value());
                std::cout << std::floor(result) << std::endl;
            }
            else if (s.is_unbounded())
                std::cout << "unbounded" << std::endl;
            else
                std::cout << "no" << std::endl;
        }
        else if (p == 2)
        {
            Program lp (CGAL::LARGER, false, 0, true, 0);

            int i = 0;

            // x + y >= -f
            lp.set_a(X, i, 1);  lp.set_a(Y, i, 1); lp.set_b(i++, -4);

            // 4x + 2y + z >= -ab
            lp.set_a(X, i, 4);  lp.set_a(Y, i, 2); lp.set_a(Z, i, 1); lp.set_b(i++, -a * b);

            // -x + y >= -1
            lp.set_a(X, i, -1); lp.set_a(Y, i, 1); lp.set_b(i++, -1);

            // objective function
            // minimize ax + by + z
            lp.set_c(X, a); lp.set_c(Y, b); lp.set_c(Z, 1);

            Solution s = CGAL::solve_linear_program(lp, ET());

            if (s.is_optimal())
            {
                double result = CGAL::to_double(s.objective_value());
                std::cout << std::ceil(result) << std::endl;
            }
            else if (s.is_unbounded())
                std::cout << "unbounded" << std::endl;
            else
                std::cout << "no" << std::endl;
        }

        std::cin >> p;
    }

    return 0;
}
