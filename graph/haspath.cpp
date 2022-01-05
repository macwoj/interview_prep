#include <vector>
#include <unordered_map>
#include <iostream>

using namespace std;
using vecT = vector<string>;
using mapT = unordered_map<string,vecT>;

bool hasPath(const mapT& graph, string src, string dst) {
    if (src == dst) return true;
    for (auto const n:graph.at(src)) {
        if (hasPath(graph,n,dst))
            return true;
    }
    return false;
}

int main() {
    {
        mapT graph {
        { "f", {"g", "i"} },
        { "g", {"h"} },
        { "h", {} },
        { "i", {"g", "k"} },
        { "j", {"i"} },
        { "k", {} }
        };

        cout << hasPath(graph, "f", "k") << endl; // 1 (true)
    }
    {
        mapT graph {
            { "f", { "g", "i" } },
            { "g", {"h"} },
            { "h", {} },
            { "i", {"g", "k"} },
            { "j", {"i"} },
            { "k", {} }
        };
        cout << hasPath(graph, "f", "j") << endl; // 0 (false)
    }
}