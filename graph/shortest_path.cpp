#include <vector>
#include <tuple>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <queue>
#include <iostream>
#include <limits>

using namespace std;
using vecT = vector<string>;
using mapT = unordered_map<string,vecT>;
using setT = unordered_set<string>;
using itemT = tuple<string,int>;
using queT = queue<itemT>;

mapT toGraph(std::vector<std::tuple<std::string, std::string>> edges) {
    mapT graph;
    //unpack directly from the range for loop
    for (auto [a,b]:edges) {
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    return graph;
}

int shortestPath(std::vector<std::tuple<std::string, std::string>> edges, std::string nodeA, std::string nodeB) {
    mapT graph = toGraph(edges);
    setT visited;
    queT q;
    q.push({nodeA,0});
    int path = -1;
    while(!q.empty()) {
        auto [node,distance] = q.front();
        q.pop();
        if (visited.find(node)!=visited.end()) 
            continue;
        visited.insert(node);
        if (node == nodeB) {
            if (path == -1 || distance < path){
                path = distance;
            }
        }
        for (auto n:graph.at(node)) {
            q.push({n,distance+1});
        }
    }
    return path;
}

int main() {
    {
        std::vector<std::tuple<std::string, std::string>> edges {
        {"w", "x"},
        {"x", "y"},
        {"z", "y"},
        {"z", "v"},
        {"w", "v"}
        };
        cout << shortestPath(edges, "w", "z") << endl; // -> 2
    }

    {
        std::vector<std::tuple<std::string, std::string>> edges {
        {"w", "x"},
        {"x", "y"},
        {"z", "y"},
        {"z", "v"},
        {"w", "v"}
        };
        cout << shortestPath(edges, "y", "x") << endl; // -> 1
    }
    {
        std::vector<std::tuple<std::string, std::string>> edges {
        {"a", "c"},
        {"a", "b"},
        {"c", "b"},
        {"c", "d"},
        {"b", "d"},
        {"e", "d"},
        {"g", "f"}
        };
        cout << shortestPath(edges, "a", "e") << endl; // -> 3
    }
}