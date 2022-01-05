#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using namespace std;
using intT = long long;
using keyT = tuple<intT,intT>;
struct hasher {
    size_t operator()(const keyT& key) const {
        auto [a,b] = key;
        return hash<intT>()(a) ^ hash<intT>()(b);
    }
};

using mapT = unordered_map<keyT,intT,hasher>;
using pmapT = shared_ptr<mapT>;

int longestSubsequence(string a, string b,size_t i=0, size_t j=0,
    pmapT memo=make_shared<mapT>()) {
    if (i==a.size() || j==b.size())
        return 0;
    auto it = memo->find({i,j});
    if (it != memo->end()) {
        return it->second;
    }
    if (a[i] == b[j]) {
        auto res = 1 + longestSubsequence(a,b,i+1,j+1,memo);
        memo->insert({{i,j},res});
        return res;
    } else {
        auto res = max(longestSubsequence(a,b,i,j+1,memo),
            longestSubsequence(a,b,i+1,j,memo));
        memo->insert({{i,j},res});
        return res;
    }
}

int main() {
    cout << longestSubsequence("ABCDGH", "AEDFHR") << endl; //ADH
    cout << longestSubsequence("AGGTAB", "GXTXAYB") << endl; //GTAB
    cout << longestSubsequence("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAGGTAB", 
        "ZZZZZZZZZZZZZZZZZZZGXTXAYB") << endl; //GTAB
}