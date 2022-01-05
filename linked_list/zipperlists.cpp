#include <string>
#include <iostream>

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



Node* zipperLists(Node* head1, Node* head2) {
    Node* curr1 = head1;
    Node* curr2 = head2;
    unsigned long long bit = 1;
    while (curr1!=nullptr && curr2!=nullptr) {
        if (bit%2 == 0) {
            auto next = curr2->next;
            curr2->next = curr1;
            curr2 = next;
        } else {
            auto next = curr1->next;
            curr1->next = curr2;
            curr1 = next;
        }
        bit++;
    }
    return head1;
}

void print(Node* node) {
    while (node) {
        cout << node->val << " ";
        node = node->next;
    }
    cout << endl;
}

int main() {
    {
        Node a("a");
        Node b("b");
        Node c("c");
        a.next = &b;
        b.next = &c;
        // a -> b -> c

        Node x("x");
        Node y("y");
        Node z("z");
        x.next = &y;
        y.next = &z;
        // x -> y -> z
        print(&a);
        print(&x);
        print(zipperLists(&a, &x));
        // a -> x -> b -> y -> c -> z
    }
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

        Node x("x");
        Node y("y");
        Node z("z");
        x.next = &y;
        y.next = &z;
        // x -> y -> z

        print(&a);
        print(&x);
        print(zipperLists(&a, &x));
        // a -> x -> b -> y -> c -> z -> d -> e -> f
    }
}