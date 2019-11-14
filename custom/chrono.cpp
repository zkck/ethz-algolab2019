#include <iostream>
#include <string>
#include <vector>

#include <chrono>
typedef std::chrono::high_resolution_clock::time_point time_point;

class Chrono {
    std::vector<time_point> timings;
    std::vector<std::string> comments;
    public:
        Chrono () {
            time_point t = std::chrono::high_resolution_clock::now();
            timings.push_back(t);
        }

        void click(std::string comment) {
            time_point t = std::chrono::high_resolution_clock::now();
            comments.push_back(comment);
            timings.push_back(t);
        }

        void show() {
            if (timings.size() == 0)
                std::cout << "No clicks done." << std::endl;
            else
            {
                size_t l = 0;
                for (std::string s : comments)
                    l = s.length() > l ? s.length() : l;
                                
                for (size_t i = 0; i < timings.size() - 1; i++)
                {
                    auto duration = timings[i + 1] - timings[i];
                    long millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
                    printf("%-*s : %f\n", static_cast<int>(l), comments[i].c_str(), millis / 1e3);
                }
            }
        }
};

int fib(int n) {
	if (n == 0)
		return 1;
	else if (n == 1)
		return 1;
	else
		return fib(n - 1) + fib(n - 2);
}

int main(int argc, char const *argv[])
{
	Chrono c;

	std::cout << "enter a number" << std::endl;

	int n; std::cin >> n;

	c.click("input");

	std::cout << fib(n) << std::endl;

	c.click("computation");

	c.show();
}
