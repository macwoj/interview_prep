#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class Solution {
public:
    using freqT = unordered_map<char,int>;
    void clear(freqT& f,char c) {
        auto it = f.find(c);
        it->second--;
        if (it->second <=0)
            f.erase(it);
    }
    
    int lengthOfLongestSubstringTwoDistinct(string s) {
        int start = 0;
        int end = 0;
        freqT f;
        f[s[0]]++;
        int result = 0;
        while (start < s.length() && end < s.length()) {
            // cout << s.substr(start,end-start+1) << endl;
            if (f.size() <= 2) {
                int res = 0;
                for (auto [k,v]:f)
                    res += v;
                if (res > result)
                    result = res;
                if (end<s.length()-1) {
                    f[s[end+1]]++;
                }
                end++;
            } else {
                clear(f,s[start]);
                start++;
            }
        }
        return result;
    }
};

/*
Input: s = "ccaabbb"
Output: 5
Explanation: The substring is "aabbb" which its length is 5.
*/
int main() {

}