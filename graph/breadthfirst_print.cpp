#include <vector>
#include <unordered_map>
#include <queue>
#include <iostream>

using namespace std;
using vecT = vector<char>;
using mapT = unordered_map<char,vecT>;
using queT = queue<char>;

void breadthFirstPrint(const mapT& graph,char source) {
    queT queue{{source}};
    while (!queue.empty()) {
        auto current = queue.front();
        queue.pop();
        cout << current << endl;
        for (auto const n:graph.at(current)) {
            queue.push(n);
        }
    }
}

int main() {
    mapT graph = {
        {'a',{'b','c'}},
        {'b',{'d'}},
        {'c',{'e'}},
        {'d',{'f'}},
        {'e',{}},
        {'f',{}}
    };
    breadthFirstPrint(graph,'a'); //acebdf
    cout << endl;
}