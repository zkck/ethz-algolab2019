#include <iostream>

// example: how to solve a simple explicit LP
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>
#include <CGAL/Gmpq.h>

// choose input type (input coefficients must fit)
typedef long IT;
// choose exact type for solver (CGAL::Gmpz or CGAL::Gmpq)
typedef CGAL::Gmpq ET;

// program and solution types
typedef CGAL::Quadratic_program<IT> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        // parsing section
        
        int n, m, h, w; std::cin >> n >> m >> h >> w;

        std::pair<IT, IT> free_nails[n];
        for (size_t i = 0; i < n; i++)
        {
            IT x, y; std::cin >> x >> y;
            free_nails[i] = std::make_pair(x, y);
        }

        std::pair<IT, IT> fixed_nails[m];
        for (size_t i = 0; i < m; i++)
        {
            IT x, y; std::cin >> x >> y;
            fixed_nails[i] = std::make_pair(x, y);
        }

        double slope = h / (double) w;

        // constraints section

        Program lp (CGAL::SMALLER, true, 1, false, 0);

        int row = 0;

        for (size_t i = 0; i < n; i++)
        {
            IT x1 = free_nails[i].first;
            IT y1 = free_nails[i].second;

            // constraints against free nails
            for (size_t j = i + 1; j < n; j++)
            {
                IT x2, y2;
                std::tie(x2, y2) = free_nails[j];

                double slope_now = (y2 - y1) / ((double) (x2 - x1));

                if (std::abs(slope_now) <= slope)
                {
                    lp.set_a(i, row, w);
                    lp.set_a(j, row, w);
                    lp.set_b(row++, 2 * std::abs(x1 - x2));
                }
                else
                {
                    lp.set_a(i, row, h);
                    lp.set_a(j, row, h);
                    lp.set_b(row++, 2 * std::abs(y1 - y2));
                }
            }

            // constraints against fixed nails
            // for (size_t j = 0; j < m; j++)
            // {
            //     IT x2, y2;
            //     std::tie(x2, y2) = fixed_nails[j];

            //     double slope_now = x2 - x1 == 0 
            //         ? (y2 - y1) / ((double) (x2 - x1))
            //         : std::numeric_limits<double>::infinity();

            //     if (std::abs(slope_now) <= slope)
            //     {
            //         if (x1 < x2)
            //         {
            //             lp.set_a(i, row, w);
            //             lp.set_b(row++, 2 * (x2 - x1) - w);
            //         }
            //         else
            //         {
            //             lp.set_a(i, row, w);
            //             lp.set_b(row++, 2 * (x1 - x2) - w);
            //         }
            //     }
            //     else
            //     {
            //         if (y1 < y2)
            //         {
            //             lp.set_a(i, row, h);
            //             lp.set_b(row++, 2 * (y2 - y1) - h);
            //         }
            //         else
            //         {
            //             lp.set_a(i, row, h);
            //             lp.set_b(row++, 2 * (y1 - y2) - h);
            //         }
            //     }
            // }
        }
        

        for (size_t i = 0; i < n; i++)
        {
            lp.set_c(i, -1);
        }

        Solution s = CGAL::solve_linear_program(lp, ET());

        // // debug
        // auto fuck = s.variable_values_begin();
        // for (size_t i = 0; i < n; i++)
        // {
        //     std::cout << "i=" << i << " " << CGAL::to_double(*(fuck + i)) << std::endl;
        // }
        

        if (s.is_optimal())
        {
            double result = -CGAL::to_double(s.objective_value()) * 2 * (h + w);
            std::cout << (int) std::ceil(result) << std::endl;
        }
        else if (s.is_unbounded())
        {
            std::cout << "unbounded" << std::endl;
        }
        else if (s.is_infeasible())
        {
            std::cout << "infeasible" << std::endl;
        }
        
        
    }
    
    return 0;
}
