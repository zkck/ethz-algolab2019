#include <iostream>
#include <string>
#include <map>
#include <vector>

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    while (num_tests-- > 0) {
        int n, q; std::cin >> n >> q;

        int ages[n];
        std::string species[n];
        std::map<std::string, size_t> indices;
        for (size_t i = 0; i < n; i++)
        {
            std::string s; int a; std::cin >> s >> a;
            indices.insert(std::make_pair(s, i));
            ages[i] = a;
            species[i] = s;
        }

        std::vector<int> parent(n, -1);
        for (size_t i = 0; i < n - 1; i++)
        {
            std::string s, p; std::cin >> s >> p;
            parent[indices.find(s)->second] = indices.find(p)->second;
        }

        for (size_t i = 0; i < q; i++)
        {
            std::string s; int b; std::cin >> s >> b;
            size_t index = indices.find(s)->second;
            while (parent[index] != -1 && ages[parent[index]] <= b) index = parent[index];
            std::cout << species[index] << (i == q - 1 ? "" : " ");
        }
        std::cout << std::endl;


    }
    return 0;
}
