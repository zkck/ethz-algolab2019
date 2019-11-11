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
    int n; std::cin >> n;
    while (n != 0)
    {
        int d; std::cin >> d;

        // distance to plane from point
        // abs(ax + by + cz + d) / length of norm
        //
        // we want a distance that is smaller than some radius
        // abs(ax + by + cz + d) / length of norm <= radius
        // 
        // expanding abs to 2 equations
        // ax + by + cz - d <=   radius * length of norm
        // ax + by + cz - d >= - radius * length of norm
        //
        // making it linear
        //  ax + by + cz - (radius * length of norm) <=   d
        // -ax - by - cz - (radius * length of norm) <= - d

        Program lp (CGAL::SMALLER, false, 0, false, 0);

        int radius = d;

        int length_of_norms[n] = { 0 };

        for (size_t i = 0; i < n; i++)
        {
            long sum = 0;
            for (size_t j = 0; j < d; j++)
            {
                int a; std::cin >> a;
                lp.set_a(j, i, a);
                sum += a * a;
            }

            length_of_norms[i] = std::sqrt(sum);
            lp.set_a(radius, i, length_of_norms[i]);
            
            int b; std::cin >> b;
            lp.set_b(i, b);
        }

        lp.set_c(radius, -1);

        Solution s = CGAL::solve_linear_program(lp, ET());

        if (s.is_optimal())
        {
            double result = -CGAL::to_double(s.objective_value());
            if (result < 0)
                std::cout << "none" << std::endl;
            else
                std::cout << (int) std::floor(result) << std::endl;
        }
        else if (s.is_unbounded())
        {
            std::cout << "inf" << std::endl;
        }
        else
        {
            std::cout << "none" << std::endl;
        }
        
        std::cin >> n;
    }
    return 0;
}
