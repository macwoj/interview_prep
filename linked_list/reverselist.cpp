#include <iostream>
#include <string>

using namespace std;

class Node {
  public:
    std::string val;
    Node* next;

    Node(std::string initialVal) {
      val = initialVal;
      next = nullptr;
    }
};

void print(Node* node) {
    while (node) {
        cout << node->val << " ";
        node = node->next;
    }
    cout << endl;
}

Node* reverseList(Node* head) {
    Node* prev = nullptr;
    Node* curr = head;
    while (curr) {
        Node* next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    return prev;
}

int main() {
    {
        Node a("a");
        Node b("b");
        Node c("c");
        Node d("d");
        Node e("e");
        Node f("f");

        a.next = &b;
        b.next = &c;
        c.next = &d;
        d.next = &e;
        e.next = &f;

        // a -> b -> c -> d -> e -> f
        print(&a);
        auto res = reverseList(&a); // f -> e -> d -> c -> b -> a
        print(res);
    }
}