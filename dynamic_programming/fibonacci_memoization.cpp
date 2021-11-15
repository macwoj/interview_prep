#include <iostream>
#include <unordered_map>
#include <memory>

using namespace std;

using intT = unsigned long long;
using mapT = unordered_map<intT,intT>;
using map_pT = shared_ptr<mapT>;

template<typename T>
void print_map(const T& m) {
    for (auto const &pair: m) {
        std::cout << "{" << pair.first << ":" << pair.second << "}";
    }
    std::cout << std::endl;
}

intT fib(intT n, map_pT m = make_shared<mapT>()) {
    auto res = m->find(n);
    if (res != m->end()) return res->second;
    if (n<=2) return 1;
    auto tmp = fib(n-1,m) + fib(n-2,m);
    m->insert({n,tmp});
    return tmp;
}

int main()
{
    std::cout << fib(6) << std::endl; //8
    std::cout << fib(7) << std::endl; //13
    std::cout << fib(8) << std::endl; //21
    std::cout << fib(50) << std::endl; //12586269025
}