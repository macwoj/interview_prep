#include <iostream>
#include <limits>
using namespace std;

class Node {
  public:
    int val;
    Node* left;
    Node* right;


    Node(int initialVal) {
      val = initialVal;
      left = nullptr;
      right = nullptr;
    }
};



int treeMinValue(Node* root) {
    if (root == nullptr) return numeric_limits<int>::max();
    int res = root->val;
    if (root->left) {
        auto left = treeMinValue(root->left);
        if (left<res)
            res = left;
    }
    if (root->right) {
        auto right = treeMinValue(root->right);
        if (right<res)
            res = right;
    }
    return res;
}


int main() {
    {
        Node a(3);
        Node b(11);
        Node c(4);
        Node d(4);
        Node e(-2);
        Node f(1);

        a.left = &b;
        a.right = &c;
        b.left = &d;
        b.right = &e;
        c.right = &f;

        //       3
        //    /    \
        //   11     4
        //  / \      \
        // 4   -2     1

        cout << treeMinValue(&a) << endl; // -> -2
    }
}