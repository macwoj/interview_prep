#include <vector>
#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include <tuple>

using namespace std;
using intT = long long;
using tupT = tuple<intT,intT>;
using gridT = vector<vector<char>>;

struct tup_hash {
    size_t operator()(const tupT& t) const {
        auto [a,b] = t;
        return hash<intT>()(a) ^ hash<intT>()(b);
    }
};

using setT = unordered_set<tupT,tup_hash>;

int explore(const gridT& grid,setT& visited,intT r,intT c) {
    if (r < 0 || r >= grid.size())
        return 0;
    if (c < 0 || c >= grid[0].size())
        return 0;
    if (grid[r][c] == 'W')
        return 0;
    auto [it,notvisited] = visited.insert({r,c});
    if (!notvisited)
        return 0;
    int count = 1;
    count += explore(grid,visited,r-1,c);
    count += explore(grid,visited,r+1,c);
    count += explore(grid,visited,r,c-1);
    count += explore(grid,visited,r,c+1);
    return count;
}

int minimumIsland(std::vector<std::vector<char>> grid) {
    intT size = numeric_limits<intT>::max();
    setT visited;
    for (intT r=0;r<grid.size();r++) {
        for (intT c=0;c<grid[0].size();c++) {
            auto res = explore(grid,visited,r,c);
            if (res > 0 && res < size) 
                size = res;
        }
    }
    return size;
}

int main() {
    {
        std::vector<std::vector<char>> grid {
        {'W', 'L', 'W', 'W', 'W'},
        {'W', 'L', 'W', 'W', 'W'},
        {'W', 'W', 'W', 'L', 'W'},
        {'W', 'W', 'L', 'L', 'W'},
        {'L', 'W', 'W', 'L', 'L'},
        {'L', 'L', 'W', 'W', 'W'}
        };
        cout << minimumIsland(grid) << endl; // -> 2
    }
    {
        std::vector<std::vector<char>> grid {
        {'L', 'W', 'W', 'L', 'W'},
        {'L', 'W', 'W', 'L', 'L'},
        {'W', 'L', 'W', 'L', 'W'},
        {'W', 'W', 'W', 'W', 'W'},
        {'W', 'W', 'L', 'L', 'L'}
        };
        cout << minimumIsland(grid) << endl; // -> 1
    }
}