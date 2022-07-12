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

int main() {

}