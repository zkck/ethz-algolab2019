#include <iostream>

int main(int argc, char const *argv[])
{
    int num_tests; std::cin >> num_tests;
    if (argc <= 1) return EXIT_FAILURE;
    int test = std::stoi(argv[1]);
    if (test >= num_tests) return EXIT_FAILURE;

    for (size_t i = 0; i < test; i++)
    {
        int n, m, s, f; std::cin >> n >> m >> s >> f;
        for (size_t j = 0; j < m; j++)
        {
            int a, b, c, d; std::cin >> a >> b >> c >> d;
        }

    }

    int n, m, s, f; std::cin >> n >> m >> s >> f;
    printf("n=%d m=%d s=%d f=%d\n", n, m, s, f);
    for (size_t j = 0; j < m; j++)
    {
        int a, b, c, d; std::cin >> a >> b >> c >> d;
        printf("  a=%d b=%d c=%d d=%d\n", a, b, c, d);
    }

    return 0;
}
