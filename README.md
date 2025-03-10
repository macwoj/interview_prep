# Daily practice
* LRU cache
  * [lrucache](code/lrucache.cpp)
* SPSC queue
  * [spsc_queue.cpp](code/spsc_queue.cpp)
  * https://www.boost.org/doc/libs/1_54_0/boost/lockfree/spsc_queue.hpp
  * https://github.com/rigtorp/SPSCQueue
  * https://gist.github.com/lc0305/6b03c8810811669eedea152708f4f03e
* Limit order book
  * [limitorderbook.cpp](code/limitorderbook.cpp)

# To know
* https://www.geeksforgeeks.org/top-10-algorithms-in-interview-questions/
* Sorting
  * merge sort
    * [sort/merge_sort.cpp][sort/merge_sort.cpp]
* algorithms
  * Cummulative algorithms
    * [largestSubgrid.cpp](code/largestSubgrid.cpp)
# C++ Reference
## tokenize
```C++
std::vector<std::string> tokenize_string_stream(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}
```
## string
### compare
* https://www.cplusplus.com/reference/string/string/compare/
* `int compare (size_t pos, size_t len, const string& str) const;` 
* str
    * Another string object, used entirely (or partially) as the comparing string.
* pos
    * Position of the first character in the current string. If this is greater than the string length, it throws out_of_range. Note: The first character is denoted by a value of 0 (not 1).
* len
    * Length of current string (if the string is shorter, as many characters as possible). A value of string::npos indicates all characters until the end of the string.
* return
    * 0	They compare equal

### substr
* https://www.cplusplus.com/reference/string/string/substr/
* `string substr (size_t pos = 0, size_t len = npos) const;`
* pos - Position of the first character to be copied as a substring.
* len - Number of characters to include in the substring, string::npos indicates all characters until the end of the string.

### find
* https://www.cplusplus.com/reference/string/string/find/
* `size_t find (const string& str, size_t pos = 0) const;`
* pos - Position of the first character in the string to be considered in the search.
* return - The position of the first character of the first match. If no matches were found, the function returns string::npos.

## map
### lower_bound/upper_bound/equal_range
* `lower_bound` - returns an iterator pointing to the first element in the range [first, last) that is greater or equal to (not less) value, or last if no such element is found.
  * index j such that value <= arr[j] and arr[j] is the smallest possible value
  * {1,2,3,4,6,7} lower_bound(5)=6 lower_bound(6)=6
* `upper_bound` - returns an iterator pointing to the first element in the range [first, last) that is greater than value, or last if no such element is found.
  * index j such that value < arr[j]
  * to get to value >= arr[j] and arr[j] is the largest possible value, we need to --it for this to be true after calling `upper_bound`
  * {1,2,3,4,6,7} upper_bound(5)=6 upper_bound(6)=7
* `equal_range` - pair::first is the lower bound of the range (the same as lower_bound), and pair::second is the upper bound (the same as upper_bound)

## prioirty queue/max heap
```C++
std::priority_queue<int> maxHeap;
maxHeap.push(20);
while (!maxHeap.empty()) {
    std::cout << maxHeap.top() << " ";
    maxHeap.pop();
}
```
- min heap
```C++
priority_queue<int, vector<int>, greater<int> > pq;
```

- custom comparator
```C++
auto compare = [](int left, int right) {
    return left < right;
};
priority_queue<int, vector<int>, decltype(compare)> pq(
    compare);
```

## heap
```C++
// Initializing a vector
vector<int> v1 = { 20, 30, 40, 25, 15 };

//O(N)
make_heap(v1.begin(), v1.end());

//to add first add to back then push_heap
//O(N)
vc.push_back(50);
//O(log)
push_heap(vc.begin(), vc.end());

// using pop_heap() function to move the largest element
// to the end
// O(logN)
pop_heap(vc.begin(), vc.end());

// actually removing the element from the heap using
// pop_back()
vc.pop_back();
```

## unordered_map

### insert
```C++
std::unordered_map<int,double> mymap;

auto it = mymap.find (key);
if ( got == mymap.end() )
  cout << "Not found";
//key
it->first
//value
it->second;

auto ret = mymap.insert({20,0.0});
//iterator to the element
ret.first
//bool -> true was inserted, false - not inserted prev value returned
ret.second
```

## set
```C++
std::set<int> myset;
auto ret = myset.insert(20);
//iterator to the element
ret.first

//bool -> true was inserted, false - not inserted prev value returned
ret.second
```

### set_intersection
```C++
set<int> intersect(const set<int>& a,const set<int>& b) {
  set<int> intersect;
  set_intersection(a.begin(),a.end(),b.begin(),b.end(),std::inserter(intersect, intersect.begin()));
  return intersect;
}
```

## queue

- inserted at back
- poped at front
```C++
queue<int> q;
//at back
q.push(10); 
q.size();
q.empty();
q.front();
q.back();
//at front
q.pop();
```

## stack
```C++
stack<int> stack;
stack.push(25);
while (!stack.empty()) {
    cout << stack.top() <<" ";
    stack.pop();
}
```

## vector
```C++
// creating
vector<int> v(size,valuetoset);
```
```C++
// min/mac value
auto min_val = *min_element(nums.begin(),nums.end());
auto max_val = *max_element(nums.begin(),nums.end());
```

## C++ Threading

### Thread-safe queue that blocks for non-full/non-empty
```C++
class BoundedBlockingQueue {
    int capacity_;
    queue<int> q_;
    mutex mtx_;
    condition_variable wait_;

public:
    BoundedBlockingQueue(int capacity) : capacity_(capacity) {}

    void enqueue(int element) {
        unique_lock<mutex> lock(mtx_);
        // wait until not full
        wait_.wait(lock, [this] { return q_.size() < capacity_; });
        // lock is aquired
        q_.push(element);
        // unlocking before notifying, to avoid waking up waiting thread only to
        // block again
        lock.unlock();
        wait_.notify_one();
    }

    int dequeue() {
        unique_lock<mutex> lock(mtx_);
        // wait until not empty
        wait_.wait(lock, [this] { return q_.size() > 0; });
        // lock is aquired
        auto res = q_.front();
        q_.pop();
        // unlocking before notifying, to avoid waking up waiting thread only to
        // block again
        lock.unlock();
        wait_.notify_one();
        return res;
    }

    int size() {
        scoped_lock<mutex> lock(mtx_);
        return q_.size();
    }
};
```

### Semaphore and barrier
- `counting_semaphore<max_count> sem{starting_count}`; 
 - ctor takes the stating count
- sem.acquire()
 - reduces count, if it reaches 0 then it gets unlocked
- sem.release() increases count

```C++
#include <barrier>

class H2O {
    counting_semaphore<1> semO;
    counting_semaphore<2> semH;
    std::barrier<> sync;
public:
    H2O():semH(2),semO(1),sync(3) {
        
    }

    void hydrogen(function<void()> releaseHydrogen) {
        semH.acquire();
        sync.arrive_and_wait();
        // releaseHydrogen() outputs "H". Do not change or remove this line.
        releaseHydrogen();
        semH.release();
    }

    void oxygen(function<void()> releaseOxygen) {
        semO.acquire();
        sync.arrive_and_wait();
        // releaseOxygen() outputs "O". Do not change or remove this line.
        releaseOxygen();
        semO.release();
    }
};
```

### SPSC Queue
```C++
template <class T, size_t N> 
class LockFreeQueue {
  std::array<T, N> buffer_{};   // Used by both threads
  std::atomic<size_t> size_{0}; // Used by both threads
  size_t read_pos_{0};          // Used by reader thread
  size_t write_pos_{0};         // Used by writer thread
  static_assert(std::atomic<size_t>::is_always_lock_free);

  bool do_push(T&& t) {
    if (size_.load() == N) {
      return false;
    }
    buffer_[write_pos_] = std::forward<decltype(t)>(t);
    write_pos_ = (write_pos_ + 1) % N;
    size_.fetch_add(1);
    return true;
  }

public:
  // Writer thread
  bool push(T&& t) { return do_push(std::move(t)); }
  bool push(const T& t) { return do_push(t); }

  // Reader thread
  auto pop() -> std::optional<T> {
    auto val = std::optional<T>{};
    if (size_.load() > 0) {
      val = std::move(buffer_[read_pos_]);
      read_pos_ = (read_pos_ + 1) % N;
      size_.fetch_sub(1);
    }
    return val;
  }
  auto size() const noexcept { return size_.load(); }
};
```

## Preorder DFS using stack

```C++
// Preorder DFS using stack
void preorderDFS(TreeNode* root) {
    if (!root) return; // Base case: empty tree
    
    stack<TreeNode*> s;
    s.push(root);

    while (!s.empty()) {
        TreeNode* node = s.top();
        s.pop();
        
        cout << node->val << " "; // Visit the node

        // Push right child first, so left child is processed first
        if (node->right) s.push(node->right);
        if (node->left) s.push(node->left);
    }
}
```

# Dynamic Programming
## Dynamic Programming - Learn to Solve Algorithmic Problems & Coding Challenges
  * https://youtu.be/oBt53YbR9Kk

## Fibonacci
Write a function fib(n) that takes in a number as an argument. The function should return the n-th number of the Fibonacci sequence.

The 1st and 2nd number of the sequence is 1. To generare the next number of the sequence, we sum the previous two.

[Solution](dynamic_programming/fibonacci_memoization.cpp)

### Brute force
O(2^n) time complexity

O(n) space complexity

### Memoization
O(2n) -> O(n) time complexity

O(n) space complexity

## Grid traveler
Say that you are a traveler on a 2D grid. You begin in the top-left corner and your goal is to travel to the bottom-right corner. You may only move dowm or right.

In how many ways can you travel to the goal on a grid with dimensions m * n?

Write a function gridTraveler(m,n) that calculates this.

[Solution](dynamic_programming/grid_traveler_memoization.cpp)
[Solution w/ string](dynamic_programming/grid_traveler_memoization_string.cpp)

### Brute force
O(2^(n+m)) time complexity , n+m is the height of the tree, we split by 2 (right or down)

O(n+m) space complexity

### Memoization
O(m*n) time complexity

O(n+m) space complexity

## Can sum
Write a function canSum(targetSum, numbers) that takes in a targetSum and an array of numbers as arguments.

The function should return a boolean indicating whether or not it is possible to generate the targetSum using numbers from the array.

You may use an element of the array as many times as needed.

You may assume that all input numbers are nonnegative.

[Solution](dynamic_programming/cansum_memoization.cpp)

### Brute force
m = target sum
n = array length

O(n^m) time

O(m) space

### Memoization
O(n*m) time

O(m) space

## How sum
Write a function howSum(targetSum, numbers) that takes in a targetSum and an array of numbers as arguments.

The function should return an array containing any combination of elements that add up to exactly the targetSum. If there is no combination that adds up to the targetSum, then return null.

If there are multiple combinations possible, you may return any single one.

[Solution](dynamic_programming/howsum_memoization.cpp)

### Brute force
m = target sum
n = array length

O(n^m * m) time, extra m comes from copying the vector

O(m) space

### Memoization
O(n*m^2) time, extra m comes from copying the vector

O(m^2) space, need store the results which have max value of m

## Best sum
Write a function bestSum(targetSum,numbers) that takes in a targetSym and an array of numbers of arguments.

The function should return an array containing the shortest combination of numbers that add up to exactly the targetSum.

If there is a tie for the shortest combination, you may return any one of the shortest.

[Solution](dynamic_programming/bestsum_memoization.cpp)

### Brute force
m = target sum
n = array length

O(n^m * m) time, extra m comes from copying the vector

O(m^2) space, storing the best adds another m

### Memoization
O(n*m^2) time, extra m comes from copying the vector

O(m^2) space

## Can construct
Write a function canConstruct(target,wordBank) that accepts a target string and an array of strings.

The function should return a boolean indicating whether or not the target can be constructed by concatenating elements of the wordBank array.

You may reuse elements of wordBank as many times as needed.

[Solution](dynamic_programming/canconstruct_memoization.cpp)

### Brute force
m = target length
n = array length

O(n^m * m) time, extra m comes from copying the vector

O(m^2) space , substr returns new string that needs to be maintainedn throughout the recursion

### Memoization
O(n*m^2) time, extra m comes from copying the string

O(m^2) space

## Count construct

Write a function countConstruct(target,wordBank) that accepts a target string and an array of strings.

The function should return the number of ways that the target can be constructed by concatenating element of the wordbank array.

You may reuse elements of wordBank as many times as needed.

[Solution](dynamic_programming/countconstruct_memoization.cpp)

### Brute force
m = target length
n = array length

O(n^m * m) time, extra m comes from copying the vector

O(m^2) space , substr returns new string that needs to be maintainedn throughout the recursion

### Memoization
O(n*m^2) time, extra m comes from copying the string

O(m^2) space

## All construct
Write a function allConstruct(target,wordBank) that accepts a target string and an array of strings.

The function should return a 2D array containing all of the ways that the target can be constructed by concatenating elements of the wordBank array. Each element of the 2D array should represent one combination that constructs the target.

You may reuse elements of wordBank as many times as needed.

[Solution](dynamic_programming/allconstruct_memoization.cpp)

### Brute force
m = target length
n = word bank length

TODO - figure this out

### Memoization
TODO - this was the O given but need to further look into it

O(n^m)

O(m) space

## TODO Tabulation

## Top 20 Dynamic Programming Interview Questions
* https://www.geeksforgeeks.org/top-20-dynamic-programming-interview-questions/

### Longest Common Subsequence
* https://www.geeksforgeeks.org/longest-common-subsequence-dp-4/
* LCS Problem Statement: Given two sequences, find the length of longest subsequence present in both of them. A subsequence is a sequence that appears in the same relative order, but not necessarily contiguous. For example, “abc”, “abg”, “bdf”, “aeg”, ‘”acefg”, .. etc are subsequences of “abcdefg”. 
* [cpp](dynamic_programming/longest_common_subsequence.cpp)
* Brute force - O((2^(n+m)))
    * Space - O(1)
* memoized = O(mn)
    * Space O(mn)

# Graph algorithms
* Representations
  * https://www.geeksforgeeks.org/graph-and-its-representations/
  * adjacency list preffered
    * unordered_map<key,vector<key>>
    * for each node keep track of the edges
  * adjacency matrix
* topologicalSort
  * Q4_07_Build_Order
* Graph Algorithms for Technical Interviews - Full Course
  * https://youtu.be/tWVWeAqZ0WU

## Depth first print
* need to use at for a const unordered_map
* use stack or recursion

[depth first print](graph/depthfirst_print.cpp)

## Breadth first print
* need to use at for a const unordered_map
* use queue

[breadth first print](graph/breadthfirst_print.cpp)

## Has path
 * https://structy.net/problems/has-path

Write a function, hasPath, that takes in an unordered map representing the adjacency list of a directed acyclic graph and two nodes (src, dst). The function should return a boolean indicating whether or not there exists a directed path between the source and destination nodes.

[has path](graph/haspath.cpp)

n = # nodes

n^2 = # edges

e = # edges

Time: O(e) O(n^2)

Space: O(n)

## Undirected path
* https://structy.net/problems/undirected-path

Write a function, undirectedPath, that takes in a vector of edges for an undirected graph and two nodes (nodeA, nodeB). The function should return a boolean indicating whether or not there exists a path between nodeA and nodeB.

[cpp](graph/undirected_path.cpp)

n = # nodes

n^2 = # edges

e = # edges

Time: O(e) O(n^2)

Space: O(n)

## Connected components count
* https://structy.net/problems/connected-components-count

Write a function, connectedComponentsCount, that takes in an unordered map respresenting the adjacency list for an undirected graph. The function should return the number of connected components within the graph.

[cpp](graph/connected_components_count.cpp)

n = # nodes

n^2 = # edges

e = # edges

Time: O(e) O(n^2)

Space: O(n)

## Largest component
* https://structy.net/problems/largest-component

Write a function, largestComponent, that takes in an unordered_map representing the adjacency list of an undirected graph. The function should return the size of the largest connected component in the graph.

[cpp](graph/largest_component.cpp)

n = # nodes

n^2 = # edges

e = # edges

Time: O(e) O(n^2)

Space: O(n)

## Shortest path
* https://structy.net/problems/shortest-path

Write a function, shortestPath, that takes in a vector of edges for an undirected graph and two nodes (nodeA, nodeB). The function should return the length of the shortest path between A and B. Consider the length as the number of edges in the path, not the number of nodes. If there is no path between A and B, then return -1.

[cpp](graph/shortest_path.cpp)

n = # nodes

n^2 = # edges

e = # edges

Time: O(e) O(n^2)

Space: O(n)

## Island count
* https://structy.net/problems/island-count
* Write a function, islandCount, that takes in a grid containing Ws and Ls. W represents water and L represents land. The function should return the number of islands on the grid. An island is a vertically or horizontally connected region of land.
* [cpp](graph/island_count.cpp)
* r = # rows
* c = # cols
* Time O(rc)
* Space O(rc)

## Minimum island
* https://structy.net/problems/minimum-island
* Write a function, minimumIsland, that takes in a grid containing Ws and Ls. W represents water and L represents land. The function should return the size of the smallest island. An island is a vertically or horizontally connected region of land. You may assume that the grid contains at least one island.
* [cpp](graph/minimum_island.cpp)
* r = # rows
* c = # cols
* Time O(rc)
* Space O(rc)

# Linked Lists
* [Linked Lists for Technical Interviews - Full Course](https://youtu.be/Hj_rA0dhr2I)

## reverse list
* https://structy.net/problems/reverse-list
* Write a function, reverseList, that takes in a pointer to the head of a linked list as an argument. The function should reverse the order of the nodes in the linked list in-place and return the new head of the reversed linked list.
* Time O(n) Space O(1)
* [cpp](linked_list/reverselist.cpp)

## zipper lists
* https://structy.net/problems/zipper-lists
* Write a function, zipperLists, that takes in pointers to the heads of two linked lists. The function should zipper the two lists together into single linked list by alternating nodes. If one of the linked lists is longer than the other, the resulting list should terminate with the remaining nodes. The function should return a pointer to the head of the zippered linked list. Do this in-place, by mutating the original Nodes. You may assume that both input lists are non-empty.
* [cpp](linked_list/zipperlists.cpp)
* m - length of list 1, n - length of list 2
* Time O(min(m,n)) Space O(1)

# Binary tree
* https://youtu.be/fAAZixBzIAI
* tree sum
  * Write a function, treeSum, that takes in the root of a binary tree that contains number values. The function should return the total sum of all values in the tree.
  * https://structy.net/problems/tree-sum
  * [treesum.cpp](binarytree/treesum.cpp)
* Min value
  * Write a function, treeMinValue, that takes in a pointer to the root of a binary tree that contains number values. The function should return the minimum value within the tree.
  * https://structy.net/problems/tree-min-value
  * [minvalue.cpp](binarytree/minvalue.cpp)
* max root to leaf path sum
  * Write a function, maxPathSum, that takes in a pointer to the root of a binary tree that contains number values. The function should return the maximum sum of any root to leaf path within the tree. You may assume that the input tree is non-empty.
  * https://structy.net/problems/max-root-to-leaf-path-sum
  * [maxroottoleafpathsum.cpp](binarytree/maxroottoleafpathsum.cpp)

# CareerCup Financial Software Developer Interview Questions 
* https://www.careercup.com/page?pid=financial-software-developer-interview-questions
*  Given a string find biggest palindrome substring. For example for given string "AABCDCBA" output should be "ABCDCBA" and for given string "DEFABCBAYT" output should be "ABCBA".
   *  [biggestPalindromeSubstring](code/biggestPalindromeSubstring.cpp)
   *  biggestPalindrome - the first implementation does more. It can handle AABQCSDCBA and still give ABCDCBA. It does have a time complexity of O(2^N/2) and space of O(N)
   *  biggestPalindromeSubstring - O(N^2) space O(1)

# geeksforgeeks
## Maximum size square Sub-Matrix with sum less than or equals to K
* Given a square grid of integers and an integer value, maxSum, determine the maximum size of the square sub-grid where for all such sub-grids, the sum of all its elements' values is less than or equal to the value maxSum.
* https://www.geeksforgeeks.org/maximum-size-square-sub-matrix-with-sum-less-than-or-equals-to-k/
* https://www.geeksforgeeks.org/prefix-sum-2d-array/
* [largestSubgrid.cpp](code/largestSubgrid.cpp)

# Questions
* https://leetcode.com/discuss/interview-question/900369/bloomberg-onsite-top-k-stocks
  * [topkstocks.cpp](code/topkstocks.cpp)
* LRU cache
  * [lrucache](code/lrucache.cpp)
* rate limiter
  * Design a logger system that receives a stream of messages along with their timestamps. Each unique message should only be printed at most every 10 seconds (i.e. a message printed at timestamp t will prevent other identical messages from being printed until timestamp t + 10).
  * [loggerratelimit.cpp](code/loggerratelimit.cpp)
* autocomplete system
  * Design a search autocomplete system for a search engine. Users may input a sentence (at least one word and end with a special character '#'). You are given a string array sentences and an integer array times both of length n where sentences[i] is a previously typed sentence and times[i] is the corresponding number of times the sentence was typed. For each input character except '#', return the top 3 historical hot sentences that have the same prefix as the part of the sentence already typed.
  * [autocomplete.cpp](code/autocomplete.cpp)
* Serialize and Deserialize Binary Tree
  * [serializetree.cpp](code/serializetree.cpp)
* sort each level of binary tree
  * [sortlevel.cpp](binarytree/sortlevel.cpp)
* SPSC queue
  * [spsc_queue.cpp](code/spsc_queue.cpp)
* Limit order book
  * [limitorderbook.cpp](code/limitorderbook.cpp)
* return level order traversal of binary tree
  * [sortlevel.cpp](binarytree/sortlevel.cpp)
* Implement the RandomizedSet class:
  * RandomizedSet() Initializes the RandomizedSet object.
  * bool insert(int val) Inserts an item val into the set if not present. Returns true if the item was not present, false otherwise.
  * bool remove(int val) Removes an item val from the set if present. Returns true if the item was present, false otherwise.
  * int getRandom() Returns a random element from the current set of elements (it's guaranteed that at least one element exists when this method is called). Each element must have the same probability of being returned.
  * You must implement the functions of the class such that each function works in average O(1) time complexity.
* Longest Substring with At Most Two Distinct Characters
  * Given a string s, return the length of the longest substring that contains at most two distinct characters.
  * [longsubwith2.cpp](array/longsubwith2.cpp)
* Read N Characters Given Read4 II - Call multiple times
   * Given a file and assume that you can only read the file using a given method read4, implement a method read to read n characters. Your method read may be called multiple times. The API read4 reads four consecutive characters from file, then writes those characters into the buffer array buf4.
   * [read4](array/read4.cpp)
* Missing Ranges
  * You are given an inclusive range [lower, upper] and a sorted unique integer array nums, where all elements are in the inclusive range. Return the smallest sorted list of ranges that cover every missing number exactly. That is, no element of nums is in any of the ranges, and each missing number is in one of the ranges.
  * [missingrange](array/missingrange.cpp)
* Min stack
  * Design a stack that supports push, pop, top, and retrieving the minimum element in constant time. Implement the MinStack class:
  * MinStack() initializes the stack object.
  * void push(int val) pushes the element val onto the stack.
  * void pop() removes the element on the top of the stack.
  * int top() gets the top element of the stack.
  * int getMin() retrieves the minimum element in the stack.
  * You must implement a solution with O(1) time complexity for each function.
  * [minstack](code/minstack.cpp)
* Android Unlock Patterns
  * Android devices have a special lock screen with a 3 x 3 grid of dots. Users can set an "unlock pattern" by connecting the dots in a specific sequence
  * adjacent and knight moves are possible
  * up/down/diagnal 2 moves are possilbe only if middle filled in
  * Given two integers m and n, return the number of unique and valid unlock patterns of the Android grid lock screen that consist of at least m keys and at most n keys.
  * [android](code/android.cpp)
* Strobogrammatic Number II
  * Given an integer n, return all the strobogrammatic numbers that are of length n. You may return the answer in any order. A strobogrammatic number is a number that looks the same when rotated 180 degrees (looked at upside down).
  * [strobogrammatic](code/strobogrammatic.cpp)
* Sentence Screen Fitting
  * Given a rows x cols screen and a sentence represented as a list of strings, return the number of times the given sentence can be fitted on the screen.
  * [screenfit.cpp](dynamic_programming/screenfit.cpp)
* Employee Free Time
  * We are given a list schedule of employees, which represents the working time for each employee.We are given a list schedule of employees, which represents the working time for each employee.
  * Each employee has a list of non-overlapping Intervals, and these intervals are in sorted order. Return the list of finite intervals representing common, positive-length free time for all employees, also in sorted order.
  * [empfree.cpp](array/empfree.cpp)
* Database
  * [database.cpp](code/database.cpp)
# SQL
* https://www.datacamp.com/blog/sql-basics-cheat-sheet
* mysql setup
  * https://towardsdatascience.com/how-to-run-mysql-using-docker-ed4cebcd90e4
* https://www.techbeamers.com/sql-query-questions-answers-for-practice/

# Exponent Cheat Sheet
* https://blog.tryexponent.com/software-engineer-interview-cheat-sheet/

# Resources
* https://github.com/jwasham/coding-interview-university
* Ace the Trading Systems Developer Interview
