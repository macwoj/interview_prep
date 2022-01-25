#include <vector>
#include <iostream>

using namespace std;
using intT = int;
using gridT=vector<vector<intT>>;
gridT gridPrefixSum(gridT& grid) {
    if (grid.empty())
        return {};
    //here we add a row and column that are all 0's so that 
    gridT sum(grid.size()+1, vector<intT>(grid[0].size()+1,0));
    for (int r=0;r<grid.size();r++) {
        for (int c=0;c<grid[0].size();c++) {
            sum[r+1][c+1] = grid[r][c] + sum[r][c+1] + sum[r+1][c] - sum[r][c];
        }
    }
    return sum;
}

int maxSquareArea(gridT& sum, int n, intT K) {
    intT maxArea = 0;
    for (int r=0;r<sum.size()-n;r++) {
        for (int c=0;c<sum[0].size()-n;c++) {
            auto area = sum[r+n][c+n] + sum[r][c] - sum[r][c+n] - sum[r+n][c];
            if (area > K)
                return area;
            if (area > maxArea)
                maxArea = area;
        }
    }
    return maxArea;
}

int largestSubgrid(gridT& grid,intT K){
    auto sum = gridPrefixSum(grid);
    int low = 0;
    int high = min(grid.size(),grid[0].size()) - 1;
    int mid = 0;
    while (low<=high) {
        mid = (high+low)/2;
        auto area = maxSquareArea(sum,mid,K);
        if (area <= K)
            low = mid + 1;
        else
            high = mid - 1;

    }
    return mid;
}

void print(gridT grid) {
    for (auto r:grid) {
        for (auto c:r) {
            cout << c << " ";
        }
        cout << endl;
    }
}

int main() {
    {
        gridT g{
            {1,1,1,1},
            {1,1,1,1},
            {1,1,1,1},
            {1,1,1,1}
        };
        //print(gridPrefixSum(g));
        cout << largestSubgrid(g,3) << endl;
    }
    {
        gridT g{
            {2,2,2},
            {3,3,3},
            {4,4,4}
        };
        //print(gridPrefixSum(g));
        cout << largestSubgrid(g,3) << endl;
        cout << largestSubgrid(g,4) << endl;
        cout << largestSubgrid(g,13) << endl;
    }
}