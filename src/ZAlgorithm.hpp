#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <chrono>
#include <tuple>

using namespace std;

void searchZ(const string& txt, const string& pat, vector<pair<int, int>>& positions, int index, double& comparisons) {
    int m = pat.size();
    int n = txt.size();
    if (m == 0 || n < m) return;

    string concat = pat + "$" + txt;
    int l = concat.length();
    vector<int> Z(l, 0);

    int L = 0, R = 0;

    for (int i = 1; i < l; ++i) {
        if (i > R) {
            L = R = i;
            while (R < l) {
                if (R > m) comparisons++;

                if (concat[R - L] == concat[R]) {
                    R++;
                }
                else {
                    break;
                }
            }
            Z[i] = R - L;
            R--;
        }
        else {
            int k = i - L;
            if (Z[k] < R - i + 1) {
                Z[i] = Z[k];
            }
            else {
                L = i;
                while (R < l) {
                    if (R > m) comparisons++;

                    if (concat[R - L] == concat[R]) {
                        R++;
                    }
                    else {
                        break;
                    }
                }
                Z[i] = R - L;
                R--;
            }
        }

        if (Z[i] == m) {
            positions.push_back({ index, i - m - 1 });
        }
    }
}

tuple<vector<vector<tuple<int, int, int, int>>>, double, double> ZAlgorithmSearch(const vector<string>& matrix, const vector<string>& pat) {
    int mRow = matrix.size();
    int mCol = matrix[0].size();
    int n = pat.size();

    vector<string> transposed(mCol, string(mRow, ' '));
    for (int i = 0; i < mRow; i++) {
        for (int j = 0; j < mCol; j++) {
            transposed[j][i] = matrix[i][j];
        }
    }

    auto start = chrono::high_resolution_clock::now();
    double comparisons = 0;
    double total_compare = 0;
    tuple<vector<vector<tuple<int, int, int, int>>>, double, double> res;

    for (int i = 0; i < n; i++) {
        comparisons = 0;
        int m = pat[i].size();
        vector<pair<int, int>> rowPositions;
        vector<pair<int, int>> colPositions;
        vector<tuple<int, int, int, int>> temp;

        for (int j = 0; j < mRow; j++) {
            searchZ(matrix[j], pat[i], rowPositions, j, comparisons);
        }

        for (int j = 0; j < mCol; j++) {
            searchZ(transposed[j], pat[i], colPositions, j, comparisons);
        }

        if (rowPositions.size() + colPositions.size() != 0) {
            total_compare += comparisons;
            for (auto pos : rowPositions) {
                temp.push_back({ pos.first, pos.second, pos.first, pos.second + m - 1 });
            }
            for (auto pos : colPositions) {
                temp.push_back({ pos.second, pos.first, pos.second + m - 1, pos.first });
            }
        }
        get<0>(res).push_back(temp);
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    double ms = duration.count() / 1000000.0; 

    get<1>(res) = ms;
    get<2>(res) = total_compare;

    return res;
}
