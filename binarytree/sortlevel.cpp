#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <queue>
#include <deque>
#include <algorithm>

using namespace std;

template<typename T>
struct Node {
    shared_ptr<Node<T>> left_;
    shared_ptr<Node<T>> right_;
    T data_;

    Node(T val): data_(val) {}
};

using nodeT = Node<int>;
using nodePtr = shared_ptr<Node<int>>;
static inline int verbose = 1;

void print(const queue<nodePtr>& q) {
    auto tmp = q;
    while(!tmp.empty()) {
        cout << tmp.front()->data_ << " ";
        tmp.pop();
    }
    cout << endl;
}

nodePtr createTree(const vector<int> values) {
    auto it = values.begin();
    it++;
    nodePtr head = make_shared<nodeT>(values.front());
    queue<nodePtr> currLevel,nextLevel;
    currLevel.push(head);
    for (;it!=values.end();++it) {
        if (verbose > 0)
            cout << *it << endl;
        if (currLevel.empty())
            currLevel = move(nextLevel);
        if (!currLevel.empty()) {
            if (verbose>0) {
                cout << "currLevel= ";
                print(currLevel);
            }
            auto node = make_shared<nodeT>(*it);
            nextLevel.push(node);
            auto front = currLevel.front();
            if (!front->left_)
                front->left_ = node;
            else if(!front->right_) {
                front->right_ = node;
                currLevel.pop();
            }
        }
    }

    return head;
}

nodePtr sortByLevel(nodePtr head) {
    vector<nodePtr> curr,next;
    curr.push_back(head);
    while(!curr.empty()) {
        for (auto n:curr) {
            if (n->left_)
                next.push_back(n->left_);
            if (n->right_)
                next.push_back(n->right_);
        }
        sort(next.begin(),next.end(),[](nodePtr a,nodePtr b) { return a->data_ < b->data_;});
        auto n = next.begin();
        for (auto c:curr) {
            if (n==next.end()) {
                c->left_ = nullptr;
            } else {
                c->left_ = *n;
                ++n;
            }
            if (n==next.end()) {
                c->right_ = nullptr;
            } else {
                c->right_ = *n;
                ++n;
            }
        }
        curr = move(next);
    }
    return head;
}

template<typename T>
void print(T node,string prefix="",bool isLeft=false) {
    if (node) {
        std::cout << prefix << (isLeft ? "├──" : "└──" );
        // print the value of the node
        std::cout << node->data_;
        cout << std::endl;
        // enter the next tree level - left and right branch
        print(node->left_,prefix + (isLeft ? "│  " : "   "), true);
        print(node->right_, prefix + (isLeft ? "│  " : "   "),false);
    }
}

int main () {

    auto node = createTree({3,4,2,1,0,10,11,20,21});
    print(node);
    print(sortByLevel(node));

}