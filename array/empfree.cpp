#include <tuple>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

// Definition for an Interval.
class Interval {
public:
    int start;
    int end;

    Interval() {}

    Interval(int _start, int _end) {
        start = _start;
        end = _end;
    }
};

class Solution {
public:
    using vecT = vector<Interval>;
    vector<Interval> employeeFreeTime(vector<vector<Interval>> schedule) {
        vector<Interval> data;
        for (auto& v:schedule)
            data.insert(data.end(),v.begin(),v.end());
        //first we need to sort the intervals by start then end time
        std::sort(data.begin(),data.end(),[](const auto& a,const auto& b) {
            if (a.start == b.start)
                return a.end < b.end;
            return a.start < b.start;
        });
        //[[[1,3][2,4][2,5][6,7][9,12]]]
        vecT result;
        //now we will go from start and keep the highest value
        //if we detect a break that is an interval to return
        int mval = data.front().start;
        for (auto s:data) {
            if (mval >= s.start && mval <= s.end) {
                mval = s.end;
            } else {
                if (s.start > mval) {
                    result.push_back({mval,s.start});
                    mval = s.end;
                }
            }
        }
        return result;
        
    }
};

// Input: schedule = [[[1,2],[5,6]],[[1,3]],[[4,10]]]
// Output: [[3,4]]
// Explanation: There are a total of three employees, and all common
// free time intervals would be [-inf, 1], [3, 4], [10, inf].
// We discard any intervals that contain inf as they aren't finite.
// Example 2:

// Input: schedule = [[[1,3],[6,7]],[[2,4]],[[2,5],[9,12]]]
// Output: [[5,6],[7,9]]

int main() {

}