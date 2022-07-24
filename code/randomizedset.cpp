#include <vector>
#include <unordered_map>

using namespace std;
class RandomizedSet {
private:
    unordered_map<int,int> map_;
    vector<int> vals_;
public:
    RandomizedSet() {
       // srand(time()); 
    }
    
    bool insert(int val) {
        auto it = map_.find(val);
        if (it==map_.end()) {
            vals_.push_back(val);
            map_[val] = vals_.size()-1;
            return true;
        }
        return false;
    }
    
    bool remove(int val) {
        auto it = map_.find(val);
        if (it!=map_.end()) {
            auto [k,v] = *it;
            vals_[v] = vals_.back();
            vals_.pop_back();
            map_[vals_[v]] = v;
            map_.erase(val);
            return true;
        }
        return false;
    }
    
    int getRandom() {
        auto i = rand() % vals_.size();
        return vals_[i];
    }
};

int main() {

}