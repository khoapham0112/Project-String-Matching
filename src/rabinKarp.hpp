#include <vector>
#include <string>
#include <tuple>
#include <chrono>
#include <algorithm>

using namespace std;

tuple<vector<vector<tuple<int, int, int, int>>>, double, double>
rabinKarp(const vector<string>& grid, const vector<string>& keywords) {
    int R = grid.size();
    if (R == 0) return {{}, 0.0, 0};
    int C = grid[0].size();

    vector<vector<tuple<int, int, int, int>>> results(keywords.size());
    double comparisons = 0; 
    double total_compare = 0;
    bool found = false;
    
    const int BASE = 256;
    const int PRIME = 101;

    auto startTime = chrono::high_resolution_clock::now();
    
    vector<int> wordHashes(keywords.size(), 0);
    vector<int> hValues(keywords.size(), 1);

    for (size_t k = 0; k < keywords.size(); ++k) {
        int M = keywords[k].length();
        if (M == 0) continue;
        
        for (int i = 0; i < M - 1; i++) {
            hValues[k] = (hValues[k] * BASE) % PRIME;
        }
        for (int i = 0; i < M; i++) {
            wordHashes[k] = (BASE * wordHashes[k] + keywords[k][i]) % PRIME;
        }
    }

    for (size_t k = 0; k < keywords.size(); ++k) {
        int M = keywords[k].length();
        if (M == 0 || M > max(R, C)) continue; 
        comparisons = 0;
        found = false;
        
        auto searchRay = [&](int sr, int sc, int dr, int dc) {
            int L = 0, r = sr, c = sc;
            while (r >= 0 && r < R && c >= 0 && c < C) { L++; r += dr; c += dc; }
            if (L < M) return;

            int textHash = 0;
            r = sr; c = sc;
            for (int i = 0; i < M; i++) {
                textHash = (BASE * textHash + grid[r][c]) % PRIME;
                r += dr; c += dc;
            }

            int csr = sr, csc = sc; 
            int cer = sr + (M - 1) * dr, cec = sc + (M - 1) * dc; 

            for (int i = 0; i <= L - M; i++) { 
                if (wordHashes[k] == textHash) {
                    bool match = true;
                    int check_r = csr, check_c = csc;
                    for (int j = 0; j < M; j++) {
                        comparisons++; 
                        if (grid[check_r][check_c] != keywords[k][j]) { match = false; break; }
                        check_r += dr; check_c += dc;
                    }
                    if (match) 
                    {
                        found = true;
                        results[k].push_back({csr, csc, cer, cec});
                    }
                }

                if (i < L - M) {
                    textHash = (textHash - (grid[csr][csc] * hValues[k]) % PRIME + PRIME) % PRIME;
                    textHash = (BASE * textHash + grid[cer + dr][cec + dc]) % PRIME;

                    csr += dr; csc += dc; 
                    cer += dr; cec += dc;
                }
            }
        };

        for (int r = 0; r < R; r++) searchRay(r, 0, 0, 1); 
        for (int c = 0; c < C; c++) searchRay(0, c, 1, 0); 
        if(found)
        {
            total_compare += comparisons;
        }
    }

    auto endTime = chrono::high_resolution_clock::now();
    double totalTime = chrono::duration<double, std::nanoseconds>(endTime - startTime).count() / 1000000.0;

    return {results, totalTime, total_compare};
}
