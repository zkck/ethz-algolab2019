#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;

int compare_bikers(IK::Segment_2 &b1, IK::Segment_2 &b2)
{
    return b1.source().y() > b2.source().y();
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;
        std::vector<IK::Segment_2> bikers;

        for (size_t i = 0; i < n; i++)
        {
            long y0, x1, y1; std::cin >> y0 >> x1 >> y1;
            IK::Segment_2 biker(IK::Point_2(0, y0), IK::Point_2(x1, y1));
            bikers.push_back(biker);
        }

        std::sort(bikers.begin(), bikers.end(), compare_bikers);

        std::vector<IK::FT> ydiffs;
        for (auto &b : bikers) {
            IK::FT ydiff = b.target().y() - b.source().y();
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
                    switch (CGAL::compare_slope(bikers[i], bikers[j]))
                    {
                    case CGAL::SMALLER:
                        std::cout << "  " << bikers[i] << " is smaller than " << bikers[j] << std::endl;
                        break;
                    case CGAL::LARGER:
                        std::cout << "  " << bikers[i] << " is larger than " << bikers[j] << std::endl;
                        break;
                    case CGAL::EQUAL:
                        std::cout << "  " << bikers[i] << " is equal than " << bikers[j] << std::endl;
                        break;
                    
                    default:
                        break;
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
