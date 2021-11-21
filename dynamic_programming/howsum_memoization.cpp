#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>

using namespace std;
using intT = long long;
using vecT = vector<intT>;
using vec_pT = shared_ptr<vecT>;
using mapT = unordered_map<intT,vec_pT>;
using map_pT = shared_ptr<mapT>;

vec_pT howSum(intT targetSum, const vecT& numbers, map_pT memo = make_shared<mapT>()) {
    auto it = memo->find(targetSum);
    if (it!=memo->end()) return it->second;
    if (targetSum == 0) return make_shared<vecT>();
    if (targetSum < 0) return nullptr;
    for (auto const i:numbers){
        auto remainder = targetSum - i;
        auto result = howSum(remainder,numbers, memo);
        if (result != nullptr){
            auto tmp = make_shared<vecT>(vecT({i}));
            tmp->insert(tmp->end(),result->begin(),result->end());
            memo->insert({targetSum,tmp});
            return tmp;
        }
    }
    memo->insert({targetSum,nullptr});
    return nullptr;
}

void print(vec_pT numbers) {
    if (numbers) {
        for (auto const i:*numbers) {
            cout << i << " ";
        }
    }
    cout << endl;
}

int main() {
    print(howSum(7,{2,3})); //3,2,2
    print(howSum(7,{5,3,4,7})); //4,3
    print(howSum(7,{2,4})); //null
    print(howSum(8,{2,3,5})); //2,2,2,2
    print(howSum(300,{7,14})); //null
}