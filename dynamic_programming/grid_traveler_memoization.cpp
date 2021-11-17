#include <iostream>
#include <unordered_map>
#include <memory>
#include <utility>

using namespace std;

struct pair_hash {
    template <typename T0,typename T1>
    size_t operator()(const pair<T0,T1>& p) const {
        return hash<T0>()(p.first) ^ hash<T1>()(p.second);
    }
};

using intT = unsigned long long;
using mapT = unordered_map<pair<intT,intT>,intT,pair_hash>;
using map_pT = shared_ptr<mapT>;

intT gridTraveler(intT m, intT n, map_pT memo=make_shared<mapT>())
{
    auto key = make_pair(m,n);
    auto it = memo->find(key);
    if (it!=memo->end()) return it->second;
    if (m == 0 || n == 0)
        return 0;
    if (m==1 || n==1)
        return 1;
    auto result = gridTraveler(m-1,n,memo) + gridTraveler(m,n-1,memo);
    memo->insert({key,result});
    return result;
}

int main()
{
    cout << gridTraveler(1, 1) << endl;   //1
    cout << gridTraveler(2, 3) << endl;   //3
    cout << gridTraveler(3, 2) << endl;   //3
    cout << gridTraveler(3, 3) << endl;   //6
    cout << gridTraveler(18, 18) << endl; //2333606220
}