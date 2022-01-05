#include <vector>
#include <unordered_map>
#include <iostream>
#include <unordered_set>
#include <tuple>

using namespace std;
using intT = long long;
using tupT = tuple<intT,intT>;
using vvecT = std::vector<std::vector<char>>;

struct tuple_hash {
    size_t operator()(const tupT& t) const {
        auto [a,b] = t;
        return hash<intT>()(a) ^ hash<intT>()(b);
    }
};

using setT = unordered_set<tupT,tuple_hash>;

bool explore(const vvecT& grid,setT& visited,intT r,intT c) {
    if (r < 0 || r >= grid.size())
        return false;
    if (c < 0 || c >= grid[0].size())
        return false;
    if (grid[r][c] == 'W')
        return false;
    auto [it,notvisited] = visited.insert({r,c});
    if (!notvisited)
        return false;
    explore(grid,visited,r-1,c);
    explore(grid,visited,r+1,c);
    explore(grid,visited,r,c-1);
    explore(grid,visited,r,c+1);
    return true;
}

int islandCount(std::vector<std::vector<char>> grid) {
    setT visited;
    int count = 0;
    for (intT r=0;r<grid.size();r++) {
        for (intT c=0;c<grid[0].size();c++) {
            if (explore(grid,visited,r,c))
                count++;
        }
    }
    return count;
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
        cout << islandCount(grid) << endl; // -> 3
    }
}