#include <unordered_map>
#include <set>
#include <vector>
#include <string>
#include <tuple>
#include <iostream>
using namespace std;

using setT = set<tuple<int,string>>;
using mapT = unordered_map<string,setT::iterator>;

mapT stocks;
setT volumes;
void addStocksVolume(string stockSymbol, int volume) {
    auto it = stocks.find(stockSymbol);
    if (it==stocks.end()) {
        auto [itVol,_] = volumes.insert({volume,stockSymbol});
        stocks.insert({stockSymbol,itVol});
    } else {
        auto vol = get<0>(*it->second);
        volumes.erase(it->second);
        auto [newIt,__] = volumes.insert({vol+volume,stockSymbol});
        it->second = newIt;
    }
}

void topKstocks(int k) {
    int i=0;
    for (auto it=volumes.rbegin();it!=volumes.rend() && i<k;it++,i++) {
        auto [volume,ticker] = *it;
        cout << ticker << " " << volume << endl;
    }
    cout << endl;
}

int main() {
    addStocksVolume("AAPL",1000);
    addStocksVolume("IBM",10);
    addStocksVolume("GE",234);
    topKstocks(3);
    addStocksVolume("META",10000);
    addStocksVolume("AAPL",4000);
    addStocksVolume("IBM",400);
    addStocksVolume("GOGL",500);
    addStocksVolume("GE",100);
    addStocksVolume("GE",100);
    addStocksVolume("M",900);
    addStocksVolume("AAPL",4000);
    topKstocks(5);
}