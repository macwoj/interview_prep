#include <unordered_map>
#include <string>
#include <iostream>

using namespace std;
using freqT = unordered_map<char,int>;
using keyT = tuple<int,int>;
struct hasher {
    size_t operator()(const keyT& key) const {
        auto [a,b] = key;
        return hash<int>()(a) ^ hash<int>()(b);
    }
};
using memoT = unordered_map<tuple<int,int>,string,hasher>;
using memoPtr = shared_ptr<memoT>;

string biggestPalindrome(string str,int s,int e,memoPtr memo=make_shared<memoT>()) {
    if (s>e || s>=str.size() || e < 0) return "";
    auto it = memo->find({s,e});
    if (it!=memo->end()) {
        auto [a,b] = it->first;
        cout << "Found " << a << "," << b << endl;
        return it->second;
    }
    if (str[s] == str[e]) {
        if (s==e) {
            auto res = str.substr(s,1);
            memo->insert({{s,e},res});
            return res;
        }
        auto res = str.substr(s,1) + biggestPalindrome(str,s+1,e-1) + str.substr(s,1);
        memo->insert({{s,e},res});
        return res;
    }
    auto res0 = biggestPalindrome(str,s+1,e);
    auto res1 = biggestPalindrome(str,s,e-1);
    if (res0.size() > res1.size()) {
        memo->insert({{s,e},res0});
        return res0;
    }
    else {
        memo->insert({{s,e},res1});
        return res1;
    }
}

string biggestPalindrome(string str) {
    return biggestPalindrome(str,0,str.size()-1);
}

string biggestPalindromeSubstring(string str) {
    int best_i = -1;
    int best_len = 0;
    for (int i=0;i<str.size();i++) {
        //assume its odd
        int l = i;
        int h = i;
        while (l >= 0 && h < str.size() && str[h]==str[l]) {
            if (best_len < h-l+1) {
                best_i = l;
                best_len = h-l+1;
            }
            l--;
            h++;
        }
        //even
        l = i;
        h = i + 1;
        while (l >= 0 && h < str.size() && str[h]==str[l]) {
            if (best_len < h-l+1) {
                best_i = l;
                best_len = h-l+1;
            }
            l--;
            h++;
        }
    }
    if (best_len>0)
        return str.substr(best_i,best_len);
    return {};
}

int main() {
    cout << biggestPalindrome("AAEFFFFBCDCBA") << endl; //ABCDCBA
    cout << biggestPalindrome("DEFAZZBCBXXAYT") << endl; //ABCBA
    cout << biggestPalindromeSubstring("AABCDCBA") << endl; //ABCDCBA
    cout << biggestPalindromeSubstring("DEFABCBAYT") << endl; //ABCBA
}