#include <tuple>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>

using namespace std;

class Solution {
public:
    int wordsTyping(vector<string>& sentence, int rows, int cols) {
        int result = 0;
        int i = 0;
        int c = cols; //8
        int start_i = i;
        int line = 0;
        unordered_map<int,tuple<int,int>> memo; //word, {how many,next word}
        while (rows>0) {
            c -= sentence[i].length();
            if (c < 0) {
                c = cols;
                rows--;
                memo.insert({start_i,{line,i}});
                start_i = i;
                line = 0;
                //check
                auto it = memo.find(start_i);
                while (it!=memo.end() && rows>0) {
                    auto [counts,next] = it->second;
                    result += counts;
                    rows--;
                    start_i = next;
                    it = memo.find(start_i);
                }
            } else {
                c--; //space
                i++;
            }
            if (i==sentence.size()) {
                result++;
                line++;
                i = 0;
            }
        }
        return result;
    }
};

// Example 1:

// Input: sentence = ["hello","world"], rows = 2, cols = 8
// Output: 1
// Explanation:
// hello---
// world---
// The character '-' signifies an empty space on the screen.
// Example 2:

// Input: sentence = ["a", "bcd", "e"], rows = 3, cols = 6
// Output: 2
// Explanation:
// a-bcd- 
// e-a---
// bcd-e-
// The character '-' signifies an empty space on the screen.
// Example 3:

// Input: sentence = ["i","had","apple","pie"], rows = 4, cols = 5
// Output: 1
// Explanation:
// i-had
// apple
// pie-i
// had--
// The character '-' signifies an empty space on the screen.

int main () {

}