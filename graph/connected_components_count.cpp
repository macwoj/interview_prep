#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

using namespace std;
using vecT = vector<int>;
using mapT = unordered_map<int,vecT>;
using setT = unordered_set<int>;


bool explore(const mapT& graph,setT& visited, int src) {
    if (visited.find(src)!=visited.end()) return false;
    visited.insert(src);
    for (auto n:graph.at(src)) {
        explore(graph,visited,n);
    }
    return true;
}

int connectedComponentsCount(mapT graph) {
  setT visited;
  int count = 0;
  for (auto it:graph) {
      if (explore(graph,visited,it.first)) {
          count++;
      }
  }
  return count;
}

int main() {
    {
        mapT graph {
        { 0, { 8, 1, 5 } },
        { 1, { 0 } },
        { 5, { 0, 8 } },
        { 8, { 0, 5 } },
        { 2, { 3, 4 } },
        { 3, { 2, 4 } },
        { 4, { 3, 2 } }
        };
        cout << connectedComponentsCount(graph) << endl; // -> 2
    }
    {
        std::unordered_map<int, std::vector<int>> graph {
        { 0, { 4, 7 } },
        { 1, { } },
        { 2, { } },
        { 3, { 6 } },
        { 4, { 0 } },
        { 6, { 3 } },
        { 7, { 0 } },
        { 8, { } },
        };
        cout << connectedComponentsCount(graph) << endl; // -> 5
    }
}