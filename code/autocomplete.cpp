#include <unordered_map>
#include <tuple>
#include <string>
#include <vector>

using namespace std;

class AutocompleteSystem {
public:
    unordered_map<string,vector<tuple<int,string>>> trie_;
    string curr_;
    unordered_map<string,int> sentences_;
    AutocompleteSystem(vector<string>& sentences, vector<int>& times) {
        for (int i=0;i<sentences.size();i++)
            sentences_[sentences[i]] = times[i];
        rebuildTrie();
    }
    
    void rebuildTrie() {
        trie_.clear();
        for (auto [k,v]:sentences_) {
            for (int i=1;i<=k.length();i++) {
                trie_[k.substr(0,i)].push_back({-v,k});
            }
        }
        for (auto& [k,v]:trie_) {
            std::sort(v.begin(),v.end());
        }
    }
    
    void rebuildTrieFast() {
        for (int i=1;i<=curr_.length();i++) {
            auto& arr = trie_[curr_.substr(0,i)];
            auto it = std::find_if(arr.begin(),arr.end(),[this](auto v) {
                auto [_,sent] = v;
                if (sent==curr_)
                    return true;
                return false;
            });
            if (it == arr.end()) {
                arr.push_back({-sentences_[curr_],curr_});
            } else {
                auto& [count,sent] = *it;
                count--;
            }
            std::sort(arr.begin(),arr.end());
        }
    }
    
    vector<string> getMatches() {
        auto it = trie_.find(curr_);
        if (it != trie_.end()) {
            vector<string> result;
            for (int i=0;i<3 && i<it->second.size();i++) {
                result.push_back(get<1>(it->second[i]));
            }
            return result;
        }
        return {};
    }
    
    vector<string> input(char c) {
        if (c == '#') {
            sentences_[curr_]++;
            rebuildTrieFast();
            curr_.clear();
           return {};
        } else {
            curr_.push_back(c);
            return getMatches();
        }
        
    }
};

/*
Input
["AutocompleteSystem", "input", "input", "input", "input"]
[[["i love you", "island", "iroman", "i love leetcode"], [5, 3, 2, 2]], ["i"], [" "], ["a"], ["#"]]
Output
[null, ["i love you", "island", "i love leetcode"], ["i love you", "i love leetcode"], [], []]

Explanation
AutocompleteSystem obj = new AutocompleteSystem(["i love you", "island", "iroman", "i love leetcode"], [5, 3, 2, 2]);
obj.input("i"); // return ["i love you", "island", "i love leetcode"]. There are four sentences that have prefix "i". Among them, "ironman" and "i love leetcode" have same hot degree. Since ' ' has ASCII code 32 and 'r' has ASCII code 114, "i love leetcode" should be in front of "ironman". Also we only need to output top 3 hot sentences, so "ironman" will be ignored.
obj.input(" "); // return ["i love you", "i love leetcode"]. There are only two sentences that have prefix "i ".
obj.input("a"); // return []. There are no sentences that have prefix "i a".
obj.input("#"); // return []. The user finished the input, the sentence "i a" should be saved as a historical sentence in system. And the following input will be counted as a new search.
*/
int main() {
}