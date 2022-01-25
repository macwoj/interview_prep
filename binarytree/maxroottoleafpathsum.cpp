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



int maxPathSum(Node* root) {
    if (root==nullptr) return -numeric_limits<int>::max();
    if (root->left==nullptr && root->right==nullptr) {
        return root->val;
    }
    return root->val + max(maxPathSum(root->left),maxPathSum(root->right));

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

    cout << maxPathSum(&a) << endl; // -> 18
}
{
    Node a(-1);
    Node b(-6);
    Node c(-5);
    Node d(-3);
    Node e(0);
    Node f(-13);
    Node g(-1);
    Node h(-2);

    a.left = &b;
    a.right = &c;
    b.left = &d;
    b.right = &e;
    c.right = &f;
    e.left = &g;
    f.right = &h;

    //        -1
    //      /   \
    //    -6    -5
    //   /  \     \
    // -3   0    -13
    //     /       \
    //    -1       -2

    cout << maxPathSum(&a) << endl; // -> -8
}
}