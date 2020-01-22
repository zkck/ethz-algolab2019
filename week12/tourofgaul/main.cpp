// Includes
// ========
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



int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n, m; std::cin >> n >> m;

        Program lp (CGAL::SMALLER, true, 0, true, 1);

        for (size_t i = 0; i < n - 1; i++)
        {
            int c; std::cin >> c;
            lp.set_b(i, c);
        }

        std::vector<size_t> fruits_in[n];
        std::vector<size_t> fruits_out[n];

        for (size_t j = 0; j < m; j++)
        {
            int a, b, d; std::cin >> a >> b >> d;
            fruits_in[a].push_back(j);
            fruits_out[b].push_back(j);
            lp.set_c(j, -d);
        }

        std::vector<int> in_pipe(m, false);

        for (size_t i = 0; i < n - 1; i++)
        {
            for (size_t f : fruits_in[i])   in_pipe[f] = true;
            for (size_t f : fruits_out[i])  in_pipe[f] = false;
            for (size_t j = 0; j < m; j++)
            {
                if (in_pipe[j]) lp.set_a(j, i, 1);
            }
        }

        Solution s = CGAL::solve_linear_program(lp, ET());
        if (s.is_optimal()) {
            std::cout << (int) -CGAL::to_double(s.objective_value()) << std::endl;
        }


    }

    return 0;
}
