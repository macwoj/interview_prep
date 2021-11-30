#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;
using intT = long long;
using mapT = unordered_map<string,intT>;
using map_pT = shared_ptr<mapT>;

intT countConstruct(string target,const vector<string>& wordBank,map_pT memo = make_shared<mapT>()) {
    auto it = memo->find(target);
    if (it!=memo->end()) return it->second;
    if (target.empty()) return 1;
    intT result = 0;
    for (auto const word:wordBank) {
        if (target.compare(0,word.size(),word) == 0){
            auto remainder = target.substr(word.size());
            auto res = countConstruct(remainder,wordBank,memo);
            result += res;
        }
    }
    memo->insert({target,result});
    return result;
}

int main() {
    cout << countConstruct("purple",{"purp","p","ur","le","purpl"}) << endl; //2
    cout << countConstruct("abcdef",{"ab","abc","cd","def","abcd"}) << endl; //1
    cout << countConstruct("skateboard",{"bo","rd","ate","t","ska","sk","boar"}) << endl; //0
    cout << countConstruct("enterapotentpot",{"a","p","ent","enter","ot","o","t"}) << endl; //4
    cout << countConstruct("eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeef",
    {"e","ee","eee","eeee","eeeee","eeeeee","eeeeeee"}) << endl; //0
}