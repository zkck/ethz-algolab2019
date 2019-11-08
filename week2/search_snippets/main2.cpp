#include <iostream>
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <limits>
using namespace std;

int main(int argc, char const *argv[])
{
    int num_tests;
    cin >> num_tests;

    while (num_tests-- > 0) {
        int num_words;
        cin >> num_words;

        int occurrences[num_words];

        for (int w = 0; w < num_words; ++w) {
            cin >> occurrences[w];
        }

        queue<int> positions[num_words];

        for (int w = 0; w < num_words; ++w) {
            for (int i = 0; i < occurrences[w]; ++i) {
                int pos; cin >> pos;
                positions[w].push(pos);
            }
        }

        // initial fly over

        int max_word_position = -1;
        int max_word = -1;

        int min_word_position = numeric_limits<int>::max();
        int min_word = -1;

        for (int w = 0; w < num_words; ++w) {
            if (positions[w].front() > max_word_position) {
                max_word_position = positions[w].front();
                max_word = w;
            }
            if (positions[w].front() < min_word_position) {
                min_word_position = positions[w].front();
                min_word = w;
            }
        }

        int next_word_position = numeric_limits<int>::max();
        int next_word = -1;

        int best_position_left  = 0;
        int best_position_right = numeric_limits<int>::max();

        do {
            for (int w = 0; w < num_words; ++w) {
                if (w == max_word)
                    continue;

                // get as close as possible to the top value
                while (positions[w].front() < max_word_position)
                    positions[w].pop();

                if (positions[w].front() > min_word_position) {
                    min_word_position = positions[w].front();
                    min_word = w;
                }

                if (max_word_position - min_word_position < best_position_right - best_position_left) {
                    best_position_left  = min_word_position;
                    best_position_right = max_word_position;
                }
            }
        } while (next_word >= 0);
        cout << min_word_position << " " << max_word_position << endl;

    }
    return 0;
}
