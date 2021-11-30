#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;

using mapT = unordered_map<string,bool>;
using map_pT = shared_ptr<mapT>;

bool canConstruct(string target,const vector<string>& wordBank, map_pT memo = make_shared<mapT>()) {
    auto it = memo->find(target);
    if (it!=memo->end()) return it->second;
    if (target.empty()) return true;
    for (auto const w:wordBank) {
        if (target.compare(0,w.size(),w) == 0) {
            auto remainder = target.substr(w.size());
            auto result = canConstruct(remainder,wordBank,memo);
            if (result) {
                memo->insert({target,true});
                return true;
            }
        }
    }
    memo->insert({target,false});
    return false;
}

int main() {
    cout << canConstruct("abcdef",{"ab","abc","cd","def","abcd"}) << endl; //true
    cout << canConstruct("skateboard",{"bo","rd","ate","t","ska","sk","boar"}) << endl; //false
    cout << canConstruct("enterapotentpot",{"a","p","ent","enter","ot","o","t"}) << endl; //true
    cout << canConstruct("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeef",
    {"e","ee","eee","eeee","eeeee","eeeeee","eeeeeee"}) << endl; //false
}