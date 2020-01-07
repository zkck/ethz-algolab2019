#include <iostream>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel IK;
typedef CGAL::Exact_predicates_exact_constructions_kernel   EK;

int compare_bikers(std::pair<EK::Segment_2, size_t> b1, std::pair<EK::Segment_2, size_t> b2) {
    return b1.first.source().y() > b2.first.source().y();
}

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0)
    {
        int n; std::cin >> n;

        std::vector<std::pair<EK::Segment_2, size_t>> bikers;
        for (size_t i = 0; i < n; i++)
        {
            long y0, x1, y1; std::cin >> y0 >> x1 >> y1;
            EK::Segment_2 biker(EK::Point_2(0, y0), EK::Point_2(x1, y1));
            bikers.push_back(std::make_pair(biker, i));
        }

        std::sort(bikers.begin(), bikers.end(), compare_bikers);

        std::vector<EK::FT> ydiffs;
        for (auto &b : bikers)
            ydiffs.push_back(b.first.point(1).y() - b.first.source().y());

        std::vector<bool> sunshine(n, true);

        size_t last = 0;
        for (size_t i = 1; i < n; i++)
        {
            size_t idx_curr = bikers[i].second, idx_last = bikers[last].second;

            EK::Segment_2 previous = bikers[last].first, current = bikers[i].first;

            EK::FT abs_curr =
                (current.target().y() - current.source().y()) /
                (current.target().x() - current.source().x());
            EK::FT abs_prev =
                (previous.target().y() - previous.source().y()) /
                (previous.target().x() - previous.source().x());

            std::cout << "  abs_curr=" << abs_curr << " abs_prev" << abs_prev << std::endl;

            auto comp = CGAL::compare_slope(previous, current);
            switch (comp)
            {
            case CGAL::SMALLER:
                std::cout << "  " << previous << " is smaller than " << current << std::endl;
                // the previous biker has smaller slope, there is a conflict
                //   -> need to compare absolute values
                if (abs_curr <= abs_prev) {
                    // previous loses, the new opponent is now current
                    std::cout << "    " << previous << " loses!" << std::endl;
                    sunshine[idx_last] = false;
                    last = i;
                } else {
                    // current loses, which means last remains the opponent
                    std::cout << "    " << current << " loses!" << std::endl;
                    sunshine[idx_curr] = false;
                }
                break;
            case CGAL::LARGER:
                std::cout << "  " << previous << " is larger than " << current << std::endl;
                last = i;
                break;
            case CGAL::EQUAL:
                std::cout << "  " << previous << " is equal to " << current << std::endl;
                last = i;
                break;
            default:
                break;
            }
        }

        for (size_t i = 0; i < n; i++)
        {
            if (sunshine[i]) std::cout << i << " ";
        }

        std::cout << std::endl;
    }

    return 0;
}
