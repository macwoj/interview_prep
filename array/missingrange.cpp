#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>

using namespace std;

class Solution {
public:
    string range(int start,int end) {
        if (start==end)
            return {};
        if (start==end-1)
            return {};
        if (start==end-2) {
            return to_string(start+1);
        }
        stringstream ss;
        ss << start+1 << "->" << end-1;
        return ss.str();
    }
    void range(vector<string>& result,int start,int end) {
        auto tmp = range(start,end);
        if (!tmp.empty())
            result.push_back(tmp);
    }
    
    vector<string> findMissingRanges(vector<int>& nums, int lower, int upper) {
        //go through each num
        //add range between nums
        //first range is lower to first num
        //last range is upper to last num
        //if range has 1 then return only one
        vector<string> result;
        if (nums.empty()) {
            range(result,lower-1,upper+1);
        } else {
            
            range(result,lower-1,nums.front());
            for (int i=0;i<nums.size()-1;i++) {
                range(result,nums[i],nums[i+1]);
            }
            range(result,nums.back(),upper+1);
        }
        return result;
    }
};

/*
Input: nums = [0,1,3,50,75], lower = 0, upper = 99
Output: ["2","4->49","51->74","76->99"]
Explanation: The ranges are:
[2,2] --> "2"
[4,49] --> "4->49"
[51,74] --> "51->74"
[76,99] --> "76->99"
*/
int main() {

}