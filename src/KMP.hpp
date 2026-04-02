#include <vector>
#include <string>
#include <tuple>
#include <chrono>

using namespace std;
using namespace std::chrono;

vector<int> computeLPS(const string& pattern) {
    int m = pattern.length();
    vector<int> lps(m, 0);
    int len = 0;
    int i = 1;
    
    while (i < m) {
        if (pattern[i] == pattern[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
    return lps;
}

tuple<vector<vector<tuple<int, int, int, int>>>, double, double>
searchGridKMP(const vector<string>& grid, const vector<string>& keywords) {
    
    vector<vector<tuple<int, int, int, int>>> all_results(keywords.size());
    double total_comparisons = 0.0;
    
    auto start_time = high_resolution_clock::now();

    if (grid.empty() || grid[0].empty() || keywords.empty()) {
        return {all_results, 0.0, 0.0};
    }

    int R = grid.size();
    int C = grid[0].size();

    for (size_t p = 0; p < keywords.size(); ++p) {
        const string& pat = keywords[p];
        int P = pat.length();
        
        if (P == 0) continue;

        vector<int> lps = computeLPS(pat);
        double local_comparisons = 0.0;
        bool found = false;

        for (int i = 0; i < R; i++) {
            int j = 0; 
            int k = 0; 

            while (j < C) {
                local_comparisons++; 
                
                if (pat[k] == grid[i][j]) {
                    j++;
                    k++;
                }

                if (k == P) {
                    int startX = i;
                    int startY = j - k;
                    int endX = i;
                    int endY = startY + P - 1;

                    all_results[p].push_back({startX, startY, endX, endY});
                    found = true;
                    k = lps[k - 1];
                } else if (j < C && pat[k] != grid[i][j]) {
                    if (k != 0) {
                        k = lps[k - 1];
                    } else {
                        j++;
                    }
                }
            }
        }

        for (int j = 0; j < C; j++) {
            int i = 0; 
            int k = 0; 

            while (i < R) {
                local_comparisons++; 
                
                if (pat[k] == grid[i][j]) {
                    i++;
                    k++;
                }

                if (k == P) {
                    int startX = i - k;
                    int startY = j;
                    int endX = startX + P - 1;
                    int endY = j;

                    all_results[p].push_back({startX, startY, endX, endY});
                    found = true;
                    k = lps[k - 1];
                } else if (i < R && pat[k] != grid[i][j]) {
                    if (k != 0) {
                        k = lps[k - 1];
                    } else {
                        i++;
                    }
                }
            }
        }

        if (found) {
            total_comparisons += local_comparisons;
        }
    }

    auto end_time = high_resolution_clock::now();
    double execution_time_ms = duration_cast<nanoseconds>(end_time - start_time).count() / 1000000.0;

    return {all_results, total_comparisons, execution_time_ms};
}
