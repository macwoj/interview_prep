#include <vector>
#include <iostream>
#include <memory>
#include <unordered_map>

using namespace std;

using intT = long long;
using vecT = vector<intT>;
using vec_pT = shared_ptr<vecT>;
using mapT = unordered_map<intT,vec_pT>;
using map_pT = shared_ptr<mapT>;

vec_pT operator+(intT i, vec_pT numbers) {
    auto tmp = make_shared<vecT>(vecT({i}));
    tmp->insert(tmp->end(),numbers->begin(),numbers->end());
    return tmp;
}

vec_pT bestSum(intT targetSum,const vecT& numbers, map_pT memo = make_shared<mapT>()) {
    auto it = memo->find(targetSum);
    if (it!=memo->end()) return it->second;
    if (targetSum < 0) return nullptr;
    if (targetSum == 0) return make_shared<vecT>();
    vec_pT best = nullptr;
    for (auto const i:numbers) {
        auto remainder = targetSum - i;
        auto result = bestSum(remainder,numbers,memo);
        if (result) {
            result = i + result;
            if (best == nullptr || result->size() < best->size()) {
                best = result;
            }
        }
    }
    memo->insert({targetSum,best});
    return best;
}

void print(vec_pT numbers) {
    if (numbers) {
        for (auto const i:*numbers) {
            cout << i << ' ';
        }
    }
 
    cout << endl;
}

int main() {
    print(bestSum(7,{5,3,4,7})); //7
    print(bestSum(8,{2,3,5})); //3 5
    print(bestSum(8,{1,4,5})); //4 4
    print(bestSum(100,{1,2,5,25})); //25 25 25 25

}