#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;

int compare_bikers(IK::Ray_2 &b1, IK::Ray_2 &b2)
{
    return b1.source().y() > b2.source().y();
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;
        std::vector<IK::Ray_2> bikers;

        for (size_t i = 0; i < n; i++)
        {
            long y0, x1, y1; std::cin >> y0 >> x1 >> y1;
            IK::Ray_2 biker(IK::Point_2(0, y0), IK::Point_2(x1, y1));
            bikers.push_back(biker);
        }

        std::sort(bikers.begin(), bikers.end(), compare_bikers);

        std::vector<IK::FT> ydiffs;
        for (auto &b : bikers) {
            IK::FT ydiff = b.point(1).y() - b.source().y();
            ydiffs.push_back(ydiff);
            std::cout << "  " << b << " is going " << ydiff << std::endl;
        }

        std::vector<int> rides_forever_into_the_sunshine;

        for (size_t i = 0; i < n; i++)
        {

            std::vector<int> range;
            if (ydiffs[i] > 0) // going up
                for (int j = 0; j < i; j++)
                    range.push_back(j);
            else if (ydiffs[i] < 0) // going down
                for (int j = i + 1; j < n; j++)
                    range.push_back(j);
            else {
                std::cout << "  straight, ignoring..." << std::endl;
                rides_forever_into_the_sunshine.push_back(i);
                continue;
            }
            std::cout << "  range size is " << range.size() << std::endl;
            bool is_cut_off = false;
            for (int j : range) {
                if (is_cut_off) break;
                if (CGAL::do_intersect(bikers[i], bikers[j])) {
                    auto o = CGAL::intersection(bikers[i], bikers[j]);
                    if (IK::Point_2 *op = boost::get<IK::Point_2>(&*o)) {
                        std::cout << "  " << *op << std::endl;
                        auto comp = CGAL::compare_distance_to_point(
                            bikers[i].source(),
                            bikers[j].source(),
                            *op
                        );
                        switch (comp)
                        {
                        case CGAL::LARGER:
                            // all good, continue
                            std::cout << "  " << bikers[i] << " beats " << bikers[j] << std::endl;
                            break;
                        case CGAL::SMALLER:
                            // the biker is cut off
                            std::cout << "  " << bikers[i] << " loses to " << bikers[j] << std::endl;
                            is_cut_off = true;
                            break;
                        case CGAL::EQUAL:
                            // same distance
                            std::cout << "  " << bikers[i] << " equal " << bikers[j] << std::endl;
                            if (ydiffs[i] < 0) is_cut_off = true;
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
            if (!is_cut_off) rides_forever_into_the_sunshine.push_back(i);
        }
        
        for (int i : rides_forever_into_the_sunshine)
        {
            std::cout << i <<  " ";
        }
        
        std::cout << std::endl;
        
        
    }
    
    return 0;
}
