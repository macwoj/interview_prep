#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <iostream>

using namespace std;
using vecT = vector<int>;
using mapT = unordered_map<int,vecT>;
using setT = unordered_set<int>;

int explore(const mapT& graph,setT& visited,int node) {
    if (visited.find(node)!=visited.end())
        return 0;
    visited.insert(node);
    int count = 1;
    for (auto n:graph.at(node)) {
        count += explore(graph,visited,n);
    }
    return count;
}

int largestComponent(mapT graph) {
  setT visited;
  int largest = 0;
  for (auto it:graph) {
      auto res = explore(graph,visited,it.first);
      if (res > largest)
        largest = res;
  }
  return largest;
}
int main() {
    {
        mapT graph {
        { 0, {8, 1, 5} },
        { 1, {0} },
        { 5, {0, 8} },
        { 8, {0, 5} },
        { 2, {3, 4} },
        { 3, {2, 4} },
        { 4, {3, 2} }
        };
        cout << largestComponent(graph) << endl; // -> 4
    }
    {
        std::unordered_map<int, std::vector<int>> graph {
        { 1, {2} },
        { 2, {1, 8} },
        { 6, {7} },
        { 9, {8} },
        { 7, {6, 8} },
        { 8, {9, 7, 2} }
        };
        cout << largestComponent(graph) << endl; // -> 6
    }
}