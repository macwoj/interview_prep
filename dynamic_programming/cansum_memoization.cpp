#include <unordered_map>
#include <iostream>
#include <utility> //hash
#include <vector>
#include <memory>

using namespace std;

using intT = long long;
using mapT = unordered_map<intT,bool>;
using map_pT = shared_ptr<mapT>;

bool canSum(intT targetSum,const vector<intT>& numbers, map_pT memo = make_shared<mapT>()) {
    auto it = memo->find(targetSum);
    if (it!=memo->end()) return it->second;
    if (targetSum == 0) return true;
    if (targetSum < 0) return false;
    for (auto i:numbers) {
        auto remainder = targetSum - i;
        auto result = canSum(remainder,numbers,memo);
        if (result) {
            memo->insert({targetSum,true});
            return true;
        }
    }
    memo->insert({targetSum,false});
    return false;
}

int main()
{
    cout << canSum(8,{2,3,5}) << endl; //true
    cout << canSum(7,{2,3}) << endl; //true
    cout << canSum(7,{5,3,4,7}) << endl; //true
    cout << canSum(7,{2,4}) << endl; //false
    cout << canSum(8,{2,3,5}) << endl; //true
    cout << canSum(300,{7,14}) << endl; //false
}