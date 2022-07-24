#include <unordered_map>
#include <tuple>
#include <string>
#include <vector>

using namespace std;

//vector for stack
//hold value and min up to this point

class MinStack {
public:
    using itemT = tuple<int,int>;
    vector<itemT> stack_;
    
    MinStack() {
        stack_.reserve(30000);
    }
    
    void push(int val) {
        if (stack_.empty()) {
            stack_.push_back({val,val});
        } else {
            auto [_,currMin] = stack_.back();
            if (val < currMin)
                currMin = val;
            stack_.push_back({val,currMin});
        }
    }
    
    void pop() {
        stack_.pop_back();
    }
    
    int top() {
        return get<0>(stack_.back());
    }
    
    int getMin() {
        return get<1>(stack_.back());
    }
};

/**
 * Your MinStack object will be instantiated and called as such:
 * MinStack* obj = new MinStack();
 * obj->push(val);
 * obj->pop();
 * int param_3 = obj->top();
 * int param_4 = obj->getMin();
 */

/*
Input
["MinStack","push","push","push","getMin","pop","top","getMin"]
[[],[-2],[0],[-3],[],[],[],[]]

Output
[null,null,null,null,-3,null,0,-2]

Explanation
MinStack minStack = new MinStack();
minStack.push(-2);
minStack.push(0);
minStack.push(-3);
minStack.getMin(); // return -3
minStack.pop();
minStack.top();    // return 0
minStack.getMin(); // return -2
*/
int main() {

}