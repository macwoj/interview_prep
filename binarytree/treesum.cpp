#include <iostream>

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



int treeSum(Node* root) {
    if (root==nullptr) return 0;
    return root->val + treeSum(root->left) + treeSum(root->right);
}

int main () {
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

        cout << treeSum(&a) << endl; // -> 21
    }

}