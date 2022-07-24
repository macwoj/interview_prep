#include <tuple>
#include <vector>
#include <iostream>

using namespace std;

class Solution {
private:
    using gridT = vector<bool>;
    using lookupT = vector<vector<int>>;
    using specialT = vector<vector<tuple<int,int>>>; //what needs to be set, next
    lookupT lookup_;
    specialT special_;
public:
    
    Solution() {
        lookup_ = {
            {},
            {2,5,4,8,6}, //1
            {1,3,4,5,6,7,9}, //2
            {2,5,6,4,8}, //3
            {1,2,5,7,8,3,9}, //4
            {1,2,3,4,6,7,8,9}, //5
            {2,3,5,8,9,1,7}, //6
            {4,5,8,2,6}, //7
            {4,5,6,7,9,1,3}, //8
            {5,6,8,2,4} //9
        };
        
        special_ = {
            {},
            {{4,7},{2,3},{5,9}}, //1
            {{5,8}}, //2
            {{2,1},{5,7},{6,9}}, //3
            {{5,6}}, //4
            {}, //5
            {{5,4}}, //6
            {{4,1},{5,3},{8,9}}, //7
            {{5,2}}, //8
            {{6,3},{5,1},{8,7}} //9
        };
    }
    int numberOfPatterns(int n,int c,gridT& grid) {
        if (n==1)
            return 1;
        int result = 0;
        grid[c] = true;
        
        for (auto next:lookup_[c]) {
            if (!grid[next])
                result += numberOfPatterns(n-1,next,grid);
        }
        
        for (auto [filled,next]:special_[c]) {
            if (!grid[next] && grid[filled])
                result += numberOfPatterns(n-1,next,grid);
        }
        
        grid[c] = false;
        return result;
    }
    
    int numberOfPatterns(int n) {
        int result = 0;
        gridT grid(10,false);
        for (int c=1;c<=9;c++) {
            result += numberOfPatterns(n,c,grid);
        }
        return result;
    }
    
    int numberOfPatterns(int m, int n) {
        if (m>n)
            return 0;
        int result = 0;
        for (int i=m;i<=n;i++)
            result += numberOfPatterns(i);
        return result;
    }
};

int main() {
    Solution s;
    cout << (s.numberOfPatterns(1,1) == 9) << endl;
    cout << (s.numberOfPatterns(2,2) == 56) << endl;
    cout << (s.numberOfPatterns(3,3) == 320) << endl;
}