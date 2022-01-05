#include <vector>
#include <unordered_map>
#include <iostream>
#include <string>
#include <unordered_set>

using namespace std;
using vecT = vector<string>;
using mapT = unordered_map<string,vecT>;
using setT = unordered_set<string>;

mapT toDirectedGraph(std::vector<std::tuple<std::string, std::string>> edges) {
    mapT graph;
    //unpack directly from the range for loop
    for (auto [a,b]:edges) {
        graph[a].push_back(b);
        graph[b].push_back(a);
    }
    // for (auto it:graph) {
    //     cout << it.first << ":";
    //     for (auto n:it.second) {
    //         cout << " " << n;
    //     }
    //     cout << endl;
    // }
    return graph;
}

bool hasPath(const mapT& graph,setT& visited, string src, string dst) {
    if (src==dst) return true;
    if (visited.find(src) != visited.end()) return false;
    visited.insert(src);
    for (auto n:graph.at(src)) {
        if (hasPath(graph,visited,n,dst))
            return true;
    }
    return false;
}

bool undirectedPath(std::vector<std::tuple<std::string, std::string>> edges, std::string src, std::string dst) {
    auto graph = toDirectedGraph(edges);
    setT visited;
    return hasPath(graph,visited,src,dst);
}

int main() {
    {
        std::vector<std::tuple<std::string, std::string>> edges {
        {"i", "j"},
        {"k", "i"},
        {"m", "k"},
        {"k", "l"},
        {"o", "n"}
        };

        cout << undirectedPath(edges, "j", "m") << endl; // -> 1 (true)
    }
}