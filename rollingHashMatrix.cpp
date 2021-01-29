#include <iostream>
#include <vector>
#define mod 1000000007
#define B 347
#define ll long long
using namespace std;

vector<vector<char>> matrix, pat;
vector<int> hashPatCol, hashMatCol;
vector<pair<int, int>> ans;
ll n, m, height, w, hashValuePat, hashValueMat, powerFinalRow, powerFinalCol;

void calcHashForCol(int row,int col,vector<int>&hash,vector<vector<char>> &grid) { // Find hash values for each column of size height
    ll h = 0, power = 1;
    for (int i = row - 1; i >= 0; i--)
    {
        h = h + (grid[i][col] * power) % mod;
        h = h % mod;
        power = (power * B) % mod;
    }
    hash[col] = h;
}

void calcHashForRow(vector<int>&hash,bool pat) { // Find hash values for first w columns
    ll h = 0, power = 1;
    for (int i = w - 1; i >= 0; i--)
    {
        h = h + (hash[i] * power) % mod;
        h = h % mod;
        power = (power * B) % mod;
    }
    if(pat) hashValuePat = h;
    else hashValueMat = h;
}

bool check(int row, int col)
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < w; j++)
        {
            if (pat[i][j] != matrix[i + row][j + col])
                return false;
        }
    }
    return true;
}

void rollCol(int row, int col) // Roll the column values downwards
{
    hashMatCol[col] = (hashMatCol[col] - (matrix[row - height][col] * powerFinalCol) % mod) % mod;
    hashMatCol[col] = (hashMatCol[col] + mod) % mod;
    hashMatCol[col] = (hashMatCol[col] * B) % mod;
    hashMatCol[col] = (hashMatCol[col] + matrix[row][col]) % mod;
}

void rollRow(int rowNumber) // Roll the row values rightwards
{
    if (hashValueMat == hashValuePat)
    {
        if (check(rowNumber, 0))
        {
            ans.push_back({rowNumber, 0});
        }
    }

    for (int i = w; i < m; i++)
    {
        hashValueMat = (hashValueMat - (hashMatCol[i - w] * powerFinalRow) % mod) % mod;
        hashValueMat = (hashValueMat + mod) % mod;
        hashValueMat = (hashValueMat * B) % mod;
        hashValueMat = (hashValueMat + hashMatCol[i]) % mod;
        if (hashValueMat == hashValuePat)
        {
            if (check(rowNumber, i - w + 1))
            {
                ans.push_back({rowNumber, i - w + 1});
            }
        }
    }
}

int main()
{

    cin >> n >> m;

    matrix.resize(n, vector<char>(m, 0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
            cin >> matrix[i][j];
    }

    cin >> height >> w;
    pat.resize(height, vector<char>(w, 0));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < w; j++)
            cin >> pat[i][j];
    }

    powerFinalRow = 1;
    for (ll i = 1; i <= w - 1; i++)
    {
        powerFinalRow = (powerFinalRow * B) % mod;
    }

    powerFinalCol = 1;
    for (ll i = 1; i <= height - 1; i++)
    {
        powerFinalCol = (powerFinalCol * B) % mod;
    }

    hashPatCol.resize(w, 0);
    for (int i = 0; i < w; i++)
        calcHashForCol(height,i,hashPatCol,pat);
    calcHashForRow(hashPatCol,true);
    
    hashMatCol.resize(m, 0);
    for (int i = 0; i < m; i++)
        calcHashForCol(height,i,hashMatCol,matrix);
    calcHashForRow(hashMatCol,false);
    rollRow(0);
    int curRow = 0;
    for (int i = height; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            rollCol(i, j);
        }
        calcHashForRow(hashMatCol,false);
        rollRow(++curRow);
    }

    for (pair<int, int> x : ans)
    {
        cout << x.first << " " << x.second << endl;
    }
}