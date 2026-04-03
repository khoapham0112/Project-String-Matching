#include <iostream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <tuple>

using namespace std;

tuple<vector<vector<tuple<int, int, int, int>>>, double, double>
bruteForce(const vector<string>& grid, const vector<string>& keywords) {

    int dr[] = { 0, 1, 0, -1 };
    int dc[] = { 1, 0, -1, 0 };

    int R = grid.size();
    int C = grid[0].size();

    double comp = 0;
    vector<vector<tuple<int, int, int, int>>> all_occurrences;

    auto start = chrono::high_resolution_clock::now();

    for (const string& word : keywords) {
        vector<tuple<int, int, int, int>> word_matches;
        int len = word.length();
        double comparisons = 0;
        for (int r = 0; r < R; ++r) {
            for (int c = 0; c < C; ++c) {
                for (int d = 0; d < 4; ++d) {
                    int end_r = r + (len - 1) * dr[d];
                    int end_c = c + (len - 1) * dc[d];

                    if (end_r >= 0 && end_r < R && end_c >= 0 && end_c < C) {
                        bool match = true;
                        for (int i = 0; i < len; ++i) {
                            comparisons++;
                            if (grid[r + i * dr[d]][c + i * dc[d]] != word[i]) {
                                match = false;
                                break;
                            }
                        }
                        if (match) {
                            word_matches.push_back(make_tuple(r, c, end_r, end_c));
                        }
                    }
                }
            }
        }
        if (!word_matches.empty()) comp += comparisons;
        all_occurrences.push_back(word_matches);
    }

    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, std::milli> duration = end - start;

    return make_tuple(all_occurrences, duration.count(), comp);
}