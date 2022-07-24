#include <tuple>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

class Solution {
public:
    unordered_map<char,string> lookup_;
    
    Solution() {
        lookup_ = {
            {'0',"0"},
            {'1',"1"},
            {'8',"8"},
            {'6',"9"},
            {'9',"6"},
        };
    }
    
    void add(vector<string>& result,vector<string> arr,char c) {
        for (auto& v:arr) {
            v += c;
        }
        result.insert(result.end(),arr.begin(),arr.end());
    }
    
    vector<string> findStrobogrammatic(int n,int i) {
        if (i==0) {
            return {"6","9","8","1"};
        }
        vector<string> result;
        auto res = findStrobogrammatic(n,i-1);
        bool isOdd = n%2==1;
        if (isOdd && i==n/2) {
            add(result,res,'1');
            add(result,res,'0');
            add(result,res,'8');
        } else if (i<n/2) {
            add(result,res,'1');
            add(result,res,'8');
            add(result,res,'9');
            add(result,res,'6');
            add(result,res,'0');
        } else {
            result = res;
            for (auto& r:result) {
                int idx = n - 1 - i;
                r += lookup_[r[idx]];
            }
        }
        
        return result;
    }
    
    vector<string> findStrobogrammatic(int n) {
        
        if (n==0)
            return {};
        else if (n==1) {
            return {"0","1","8"};
        } else {
            return findStrobogrammatic(n,n-1);
        }
        
        // 111,888
        //609,906,
        // 1111,6699,8888,9966
        //11111  3->1 4->0
    }
};

// Input: n = 2
// Output: ["11","69","88","96"]
//3
//["101","111","181","609","619","689","808","818","888","906","916","986"]
//4
//["1001","1111","1691","1881","1961","6009","6119","6699","6889","6969","8008","8118","8698","8888","8968","9006","9116","9696","9886","9966"]
int main() {

}