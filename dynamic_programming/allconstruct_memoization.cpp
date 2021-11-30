#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

using namespace std;

using vecT = vector<string>;
using vec_pT = shared_ptr<vecT>;
using vvecT = vector<vecT>;
using vvec_pT = shared_ptr<vvecT>;
using mapT = unordered_map<string,vvec_pT>;
using map_pT = shared_ptr<mapT>;

void print(vvec_pT result) {
    if (!result)
        return;
    cout << "[ ";
    for (auto i:*result) {
        cout << "[ ";
        for (auto j:i) {
            cout << "\"" << j << "\" ";
        }
        cout << "] ";
    }
    cout << " ]" << endl;
}

vvec_pT addWord(string word, vvec_pT array) {
    vvec_pT result = make_shared<vvecT>(*array);
    for (auto& it:*result) {
        it.insert(it.begin(), word);
    }
    return result;
}

vvec_pT allConstruct(string target,const vecT& wordBank,map_pT memo = make_shared<mapT>()) {
    auto it = memo->find(target);
    if (it!=memo->end())
        return it->second;
    if (target.empty()) {
        return make_shared<vvecT>(vvecT({vecT()}));
    }
    vvec_pT result = make_shared<vvecT>();
    for (auto word:wordBank) {
        //need to find the root matches
        if (target.compare(0,word.size(),word)==0){
            //call underlying with root removed
            auto remainder = target.substr(word.size());
            auto res = allConstruct(remainder,wordBank,memo);
            //if we can construct the whole word we need to add 
            // the current prefix to result at begining of each result.
            if (!res->empty()) {
                auto tmp = addWord(word,res);
                result->insert(result->end(),tmp->begin(),tmp->end());
            }
        }
    }
    memo->insert({target,result});
    return result;
}

int main() {
    //[ ["purp","le"],["p","ur","p","le"] ]
    print(allConstruct("purple",{"purp","p","ur","le","purpl"}));
    //[ [ab cd ef] [ab c def] [abc def] [abcd ef] ]
    print(allConstruct("abcdef",{"ab","abc","cd","def","abcd","ef","c"}));
    //[]
    print(allConstruct("skateboard",{"bo","rd","ate","t","ska","sk","boar"}));
    //[]
    print(allConstruct("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaz",{"a","aa","aaa","aaaa","aaaaa"}));
}