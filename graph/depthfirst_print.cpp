#include <vector>
#include <unordered_map>
#include <stack>
#include <iostream>

using namespace std;
using vecT = vector<char>;
using mapT = unordered_map<char,vecT>;
using staT = stack<char>;

void depthFirstPrint(const mapT& graph,char source) {
    staT stack{{source}};
    while (!stack.empty()){
        auto current = stack.top();
        stack.pop();
        cout << current;
        //need to use at since the map is const
        for (auto const& n:graph.at(current)) {
            stack.push(n);
        }
    }
}

void depthFirstPrintRecursive(const mapT& graph,char source) {
    cout << source;
    for (auto const& n:graph.at(source)) {
        depthFirstPrintRecursive(graph,n);
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
    depthFirstPrint(graph,'a'); //acebdf
    cout << endl;
    depthFirstPrintRecursive(graph,'a');
    cout << endl;
}