#include <iostream>
#include <list>
#include <unordered_map>
#include <tuple>

using namespace std;

template<typename TKey,typename TValue>
class Cache {
    using itemT = tuple<TKey,TValue>;
    using listT = list<itemT>;
    using mapT = unordered_map<TKey,typename listT::iterator>;
private:
    listT cache_;
    mapT lookup_;
    int capacity_; //TODO add to ctor
public:
    Cache(int capacity): capacity_(capacity) {}

    TValue get(const TKey& key) {
        auto res = lookup_.find(key);
        if (res!=lookup_.end()) {
            auto [key,val] = *res->second;
            cache_.splice(cache_.begin(),cache_,res->second);
            return val;
        }
        return {};
    }

    void put(const TKey& key,const TValue val) {
        auto res = lookup_.find(key);
        if (res==lookup_.end()) {
            cache_.push_front({key,val});
            auto it = cache_.begin();
            lookup_.insert({key,it});
        } else {
            std::get<1>(*res->second) = val;
            cache_.splice(cache_.begin(),cache_,res->second);
        }
        while (cache_.size() > capacity_ && !cache_.empty()) {
            auto it = cache_.end();
            it--;
            auto [key,val] = *it;
            lookup_.erase(key);
            cache_.erase(it);
        }
    }

    void print() {
        for (auto it:cache_) {
            auto [key,val] = it;
            cout << "[" << key << "," << val << "] ";
        }
        cout << endl;
    }

};

int main() {
    Cache<int,int> cache(5);
    cache.put(1,1);
    cache.put(2,2);
    cache.put(3,3);
    cache.print();
    cache.put(4,4);
    cache.put(5,5);
    cache.print();
    cache.put(6,6);
    cache.print();
    cache.get(2);
    cache.print();
    cache.get(6);
    cache.print();
    cache.get(3);
    cache.print();
    cache.put(4,10);
    cache.print();
}