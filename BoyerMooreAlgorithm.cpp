#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>
#include <chrono>
#include <tuple>

using namespace std;
#define NO_OF_CHARS 256

void badCharHeuristic(string str, int size, vector<int> &badchar)
{
    int i;

    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    for (i = 0; i < size; i++)
        badchar[(int)(unsigned char)str[i]] = i;
}

void preprocess_strong_suffix(vector<int> &shift, vector<int> &bpos, string pat, int m)
{
    int i = m, j = m + 1;
    bpos[i] = j;

    while (i > 0)
    {
        while (j <= m && pat[i - 1] != pat[j - 1])
        {
            if (shift[j] == 0)
                shift[j] = j - i;
 
            j = bpos[j];
        }
        i--; j--;
        bpos[i] = j;
    }

    j = bpos[0];
    for (int k = 0; k <= m; k++)
    {
        if (shift[k] == 0)
            shift[k] = j;
        if (k == j)
            j = bpos[j];
    }
}



void search(string txt, string pat,const vector<int>& badchar,const vector<int>& goodSuffixShift, vector<pair<int, int>>& positions, int index, double &comparisons)
{
    int m = pat.size();
    int n = txt.size();

    if (m == 0 || n < m) return;
    int s = 0;

    while (s <= (n - m)) {
        int j = m - 1;
        while (j >= 0) {
            comparisons++;
            if (pat[j] == txt[s + j]) {
                j--;
            }
            else {
                break;
            }
        }

        if (j < 0) {
            positions.push_back( {index, s});
            s += goodSuffixShift[0];
        }

        else
        {
            unsigned char temp = txt[s + j];
            int bc_shift = j - badchar[temp];
            s += max(bc_shift, goodSuffixShift[j + 1]);
        }    
    }
}

tuple<vector<vector<tuple<int, int, int, int>>>, double, double> BoyerMooreAlgorithm(std::vector<std::string> matrix, vector<string> pat)
{
    int mRow = matrix.size();
    int mCol = matrix.size();
    int n = pat.size();
    
    vector<string> transposed(mCol, string(mRow, ' '));
    for (int i = 0; i < mRow; i++) {
        for (int j = 0; j < mCol; j++) {
            transposed[j][i] = matrix[i][j];
        }
    }

    auto start = chrono::high_resolution_clock::now();
    double comparisons = 0;
    tuple<vector<vector<tuple<int, int, int, int>>>, double, double> res;
    
    for (int i = 0; i < n; i++)
    {
       
        vector<int> badchar(NO_OF_CHARS);
        int m = pat[i].size();
        badCharHeuristic(pat[i], m, badchar);

        vector<int> bpos(m + 1);
        vector<int> goodSuffixShift(m + 1);
        for (int j = 0; j < m + 1; j++) goodSuffixShift[j] = 0;
        preprocess_strong_suffix(goodSuffixShift, bpos, pat[i], m);

        vector<pair<int, int>> rowPositions;
        vector<pair<int, int>> colPositions;
        vector<tuple<int, int, int, int>> temp;
        for (int j = 0; j < mRow; j++)
        {
            search(matrix[j], pat[i], badchar, goodSuffixShift, rowPositions, j, comparisons);
        }
        for (int j = 0; j < mCol; j++)
        {
            search(transposed[j], pat[i], badchar, goodSuffixShift, colPositions, j, comparisons);
        }

        

        if (rowPositions.size() + colPositions.size() != 0)
        {
            for (pair<int, int> pos : rowPositions)
            {
                temp.push_back({ pos.first, pos.second, pos.first, pos.second + m - 1 });
            }
            for (pair<int, int> pos : colPositions)
            {
                temp.push_back({ pos.second, pos.first, pos.second + m - 1, pos.first });
            }
            
        }
        get<0>(res).push_back(temp);
    }

    auto end = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::nanoseconds>(end - start);
    double ms = duration.count() / 1000000.0;
    get<1>(res) = ms;
    get<2>(res) = comparisons;
    return res;
}
