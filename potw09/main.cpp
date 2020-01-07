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
        // PARSING SECTION

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

        // CONSTRAINTS SECTION 1: Declaration

        Program lp (CGAL::SMALLER, true, 1, false, 0);

        int row = 0;

        // CONSTRAINTS SECTION 2: Free nails constraints

        for (size_t i = 0; i < n; i++)
        {
            IT x1 = free_nails[i].first;
            IT y1 = free_nails[i].second;

            // create constraints for each unique pair of free nails

            for (size_t j = i + 1; j < n; j++)
            {
                IT x2, y2;
                std::tie(x2, y2) = free_nails[j];

                double slope_now = (y2 - y1) / ((double) (x2 - x1));

                if (std::abs(slope_now) <= slope)
                {
                    // the pair of free nails require a x-constraint
                    lp.set_a(i, row, w);
                    lp.set_a(j, row, w);
                    lp.set_b(row++, 2 * std::abs(x1 - x2));
                }
                else
                {
                    // the pair of free nails require a y-constraint
                    lp.set_a(i, row, h);
                    lp.set_a(j, row, h);
                    lp.set_b(row++, 2 * std::abs(y1 - y2));
                }
            }
        }

        // CONSTRAINTS SECTION 3: Fixed nails constraints

        // x-constraints

        for (size_t i = 0; i < n; i++)
        {
            IT x1, y1; std::tie(x1, y1) = free_nails[i];

            // find the closest fixed nails on both sides

            int left  = -1;
            int right = -1;

            IT left_x2, right_x2;

            for (size_t j = 0; j < m; j++)
            {
                IT x2, y2; std::tie(x2, y2) = fixed_nails[j];
                double slope_now = (y2 - y1) / (double) (x2 - x1);

                // check if a x-constraint is applicable

                if (std::abs(slope_now) <= slope) {
                    if (x2 < x1 && (left < 0 || left_x2 < x2)) {
                        left = j;
                        left_x2 = x2;
                    }
                    if (x1 < x2 && (right < 0 || x2 < right_x2)) {
                        right = j;
                        right_x2 = x2;
                    }
                }
            }

            if (left >= 0)
            {
                lp.set_a(i, row, w);
                lp.set_b(row++, 2 * (x1 - left_x2) - w);
            }

            if (right >= 0)
            {
                lp.set_a(i, row, w);
                lp.set_b(row++, 2 * (right_x2 - x1) - w);
            }
        }

        // y-constraints

        for (size_t i = 0; i < n; i++)
        {
            IT x1, y1; std::tie(x1, y1) = free_nails[i];

            // find the closest fixed nails above and below

            int above  = -1;
            int below = -1;

            IT above_y2, below_y2;

            for (size_t j = 0; j < m; j++)
            {
                IT x2, y2; std::tie(x2, y2) = fixed_nails[j];
                double slope_now = (y2 - y1) / (double) (x2 - x1);

                // check if y-constraint is applicable

                if (std::abs(slope_now) > slope) {
                    if (y2 < y1 && (below < 0 || below_y2 < y2)) {
                        below = j;
                        below_y2 = y2;
                    }
                    if (y1 < y2 && (above < 0 || y2 < above_y2)) {
                        above = j;
                        above_y2 = y2;
                    }
                }
            }

            if (above >= 0)
            {
                lp.set_a(i, row, h);
                lp.set_b(row++, 2 * (above_y2 - y1) - h);
            }

            if (below >= 0)
            {
                lp.set_a(i, row, h);
                lp.set_b(row++, 2 * (y1 - below_y2) - h);
            }
        }

        // OBJECTIVE FUNCTION

        for (size_t i = 0; i < n; i++)
        {
            lp.set_c(i, -1);
        }

        // SOLUTION SECTION

        Solution s = CGAL::solve_linear_program(lp, ET());

        if (s.is_optimal())
        {
            double result = -CGAL::to_double(s.objective_value()) * 2 * (h + w);
            std::cout << (long) std::ceil(result) << std::endl;
        }
    }

    return 0;
}
