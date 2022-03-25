#include <vector>
#include <iostream>

using namespace std;
using intT = int;
using gridT=vector<vector<intT>>;

static int verbose = 0;

gridT gridPrefixSum(gridT& grid) {
    if (grid.empty())
        return {};
    //here we add a row and column that are all 0's so that 
    gridT sum(grid.size(), vector<intT>(grid[0].size(),0));
    sum[0][0] = grid[0][0];
    for (int r=1;r<grid.size();r++) {
        sum[r][0] = grid[r][0] + sum[r-1][0];
    }
    for (int c=1;c<grid[0].size();c++) {
        sum[0][c] = grid[0][c] + sum[0][c-1];
    }
    for (int r=1;r<grid.size();r++) {
        for (int c=1;c<grid[0].size();c++) {
            sum[r][c] = grid[r][c] + sum[r][c-1] + sum[r-1][c] - sum[r-1][c-1];
        }
    }
    return sum;
}
void print(gridT grid) {
    for (auto r:grid) {
        for (auto c:r) {
            cout << c << " ";
        }
        cout << endl;
    }
    cout << endl;
}
int maxSquareArea(const gridT& sum, int n, intT K) {
    if (n==0)
        return 0;
    intT maxArea = 0;
    gridT output(sum.size(), vector<intT>(sum[0].size(),-1));
    for (int r=n-1;r<sum.size();r++) {
        for (int c=n-1;c<sum[0].size();c++) {
            auto area = sum[r][c];
            bool col = c-n >= 0;
            bool row = r-n >= 0;
            if (row)
                area -= sum[r-n][c];
            if (col)
                area -= sum[r][c-n];
            if (col && row)
                area += sum[r-n][c-n];
            output[r][c] = area;
            // if (area > K)
            //     return area;
            if (area > maxArea)
                maxArea = area;
        }
    }
    if (verbose>2)
        print(output);
    return maxArea;
}

int largestSubgrid(gridT& grid,intT K){
    auto sum = gridPrefixSum(grid);
    if (verbose>1)
        print(sum);
    int low = 0;
    int high = min(grid.size(),grid[0].size());
    int mid = 0;
    //this is a variation of binary search
    //we need to find the area or the area that is less then
    // if (area <= K) this  makes the low always one more than the largest area
    //that still fits if the current search area has all areas that are too big
    //Example
    while (low<=high) {
        mid = (high+low)/2;
        auto area = maxSquareArea(sum,mid,K);
        if (verbose)
            cout << "binsearch " << low << " " << mid << " " << high << " " << area << endl;
        if (area <= K)
            low = mid + 1;
        else
            high = mid - 1;
        if (verbose)
            cout << "\tbinsearch " << low << " " << high << endl;
    }
    return low - 1;
}

int main() {
    verbose = 2;
    {
        gridT g{
            {1,1,1,1},
            {1,1,1,1},
            {1,1,1,1},
            {1,1,1,1}
        };
        //print(gridPrefixSum(g));
        // cout << largestSubgrid(g,3) << endl;
    }
    {
        gridT g{
            {2,2,2},
            {3,3,3},
            {4,4,4}
        };
        // print(gridPrefixSum(g));
        // maxSquareArea(gridPrefixSum(g),1,100);
        // maxSquareArea(gridPrefixSum(g),2,100);
        // maxSquareArea(gridPrefixSum(g),3,100);
        // cout << largestSubgrid(g,5) << endl;
        for (int i=0;i<29;i++) {
            //maxsum < 4 = 0, 4<=maxsum<14 = 1,14<=maxSum<27=2, maxsum>=27 = 3
            cout << i << " " << largestSubgrid(g,i) << endl;
        }
    }
        {
        gridT g{
            {2,2,2,2},
            {3,3,3,3},
            {4,4,4,4},
            {5,5,5,5}
        };
        // print(gridPrefixSum(g));
        // maxSquareArea(gridPrefixSum(g),1,100);
        // maxSquareArea(gridPrefixSum(g),2,100);
        // maxSquareArea(gridPrefixSum(g),3,100);
        // cout << largestSubgrid(g,5) << endl;
        for (int i=0;i<30;i++) {
            //maxsum < 4 = 0, 4<=maxsum<14 = 1,14<=maxSum<27=2, maxsum>=27 = 3
            cout << "i=" << i << "\n" << largestSubgrid(g,i) << endl;
        }
    }
}