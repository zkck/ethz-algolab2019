#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;

int compare_bikers(std::pair<IK::Ray_2, size_t> b1, std::pair<IK::Ray_2, size_t> b2) {
    return b1.first.source().y() > b2.first.source().y();
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;

        std::vector<std::pair<IK::Ray_2, size_t>> bikers;
        for (size_t i = 0; i < n; i++)
        {
            long y0, x1, y1; std::cin >> y0 >> x1 >> y1;
            IK::Ray_2 biker(IK::Point_2(0, y0), IK::Point_2(x1, y1));
            bikers.push_back(std::make_pair(biker, i));
        }

        std::sort(bikers.begin(), bikers.end(), compare_bikers);

        std::vector<EK::FT> ydiffs;
        for (auto &b : bikers)
            ydiffs.push_back(b.first.point(1).y() - b.first.source().y());

        std::vector<size_t> sunshine;

        for (size_t i = 0; i < n; i++)
        {
            size_t    index = bikers[i].second;
            IK::Ray_2 biker = bikers[i].first;

            // slope of the biker
            EK::FT biker_slope = CGAL::abs(biker.direction().dy() / biker.direction().dx());

            bool is_cut_off = false;
            if (ydiffs[i] < 0)
            {
                for (size_t j = i + 1; j < n && !is_cut_off; j++)
                {
                    IK::Ray_2 rival = bikers[j].first;
                    if (CGAL::do_intersect(biker, rival))
                    {
                        if (ydiffs[j] <= 0)
                        {
                            // if there is an intersection and both are going downwards, the
                            // biker has lost
                            is_cut_off = true;
                        }
                        else 
                        {
                            // if same distance to point, the one with the flattest slope wins,
                            // or the rival if they are equal (rival has right of way)
                            EK::FT rival_slope = CGAL::abs(rival.direction().dy() / rival.direction().dx());
                            if (rival_slope <= biker_slope)
                                is_cut_off = true;
                        }
                    }
                }
            }
            else if (ydiffs[i] > 0)
            {
                for (size_t j = 0; j < i && !is_cut_off; j++)
                {
                    IK::Ray_2 rival = bikers[j].first;
                    if (CGAL::do_intersect(biker, rival))
                    {
                        if (ydiffs[j] >= 0)
                        {
                            // if there is an intersection and both are going downwards, the
                            // biker has lost
                            is_cut_off = true;
                        }
                        else 
                        {
                            // if same distance to point, the one with the flattest slope wins,
                            // or the rival if they are equal (rival has right of way)
                            EK::FT rival_slope = CGAL::abs(rival.direction().dy() / rival.direction().dx());
                            if (rival_slope < biker_slope)
                                is_cut_off = true;
                        }
                    }

                }
            }

            if (!is_cut_off) sunshine.push_back(index);
        }

        std::sort(sunshine.begin(), sunshine.end());
        
        for (size_t i : sunshine)
            std::cout << i << " ";
        std::cout << std::endl;
    }
    
    return 0;
}
