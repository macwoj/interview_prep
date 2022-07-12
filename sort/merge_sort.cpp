#include <vector>
#include <iostream>
using namespace std;

void print(vector<int> v) {
    for (auto i:v)
        cout << i << " ";
    cout << endl;
}

void merge(vector<int>& v,int start,int piv,int end) {
    auto left = vector<int>(v.begin()+start,v.begin()+(piv+1));
    auto right = vector<int>(v.begin()+(piv+1),v.begin()+(end+1));
    print(left);
    print(right);
    auto il = left.begin();
    auto ir = right.begin();
    for (int i=start;i<=end;i++) {
        if (il!=left.end() && ir!=right.end()) {
            cout  << "il=" << *il << endl;
            cout  << "ir=" << *ir << endl;
            if (*il<*ir) {
                v[i] = *il;
                ++il;
            } else {
                v[i] = *ir;
                ++ir;
            }
        } else if (il!=left.end()) {
            cout  << "il=" << *il << endl;
            v[i] = *il;
            ++il;
        } else {
            cout  << "ir=" << *ir << endl;
            v[i] = *ir;
            ++ir;
        }
    }
    print(v);
}

void mergesort(vector<int>& v,int start,int end) {
    if (end <= start)
        return;
    int piv = (start+end) / 2;
    mergesort(v,start,piv);
    mergesort(v,piv+1,end);
    merge(v,start,piv,end);
}

vector<int> mergesort(vector<int> v) {
    mergesort(v,0,v.size()-1);
    return v;
}

int main() {
    print(mergesort({3,1,2}));
    print(mergesort({8,2,56,23,11,0,44,2,56}));
}