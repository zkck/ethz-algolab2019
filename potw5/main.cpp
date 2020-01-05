#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;
        std::vector<IK::Ray_2> bikers;

        for (size_t i = 0; i < n; i++)
        {
            unsigned long y0, x1, y1; std::cin >> y0 >> x1 >> y1;
            IK::Ray_2 biker(IK::Point_2(0, y0), IK::Point_2(x1, y1));
            bikers.push_back(biker);
        }

        std::vector<bool> rides_forever_into_the_sunrise(n, true);

        for (size_t i = 0; i < n; i++)
        {
            for (size_t j = i + 1; j < n && rides_forever_into_the_sunrise[i]; j++)
            {
                IK::Ray_2 biker1 = bikers[i], biker2 = bikers[j];
                if (CGAL::do_intersect(biker1, biker2)) {
                    auto o = CGAL::intersection(biker1, biker2);
                    if (const IK::Point_2* op = boost::get<IK::Point_2>(&*o)) {
                        auto comp = CGAL::compare_distance_to_point(
                            biker1.source(),
                            biker2.source(),
                            *op
                        );
                        switch (comp)
                        {
                        case CGAL::SMALLER:
                            // biker1 is closer to the intersection than biker2
                            rides_forever_into_the_sunrise[j] = false;
                            break;
                        case CGAL::LARGER:
                            // biker2 is closer to the intersection than biker1
                            rides_forever_into_the_sunrise[i] = false;
                            break;
                        case CGAL::EQUAL:
                            if (biker1.source().y() < biker2.source().y())
                                // biker1 is on the right of biker2
                                rides_forever_into_the_sunrise[j] = false;
                            else
                                // biker2 is on the right of biker1
                                rides_forever_into_the_sunrise[i] = false;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }
        
        for (size_t i = 0; i < n; i++)
        {
            if (rides_forever_into_the_sunrise[i])
            {
                std::cout << i << " ";
            }
        }
        
        std::cout << std::endl;
        
        
    }
    
    return 0;
}
