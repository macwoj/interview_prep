- [Special functions](#special-functions)
  - [compare](#compare)
  - [substr](#substr)
  - [find](#find)
- [Dynamic Programming](#dynamic-programming)
  - [Fibonacci](#fibonacci)
    - [Brute force](#brute-force)
    - [Memoization](#memoization)
  - [Grid traveler](#grid-traveler)
    - [Brute force](#brute-force-1)
    - [Memoization](#memoization-1)
  - [Can sum](#can-sum)
    - [Brute force](#brute-force-2)
    - [Memoization](#memoization-2)
  - [How sum](#how-sum)
    - [Brute force](#brute-force-3)
    - [Memoization](#memoization-3)
  - [Best sum](#best-sum)
    - [Brute force](#brute-force-4)
    - [Memoization](#memoization-4)
  - [Can construct](#can-construct)
    - [Brute force](#brute-force-5)
    - [Memoization](#memoization-5)
  - [Count construct](#count-construct)
    - [Brute force](#brute-force-6)
    - [Memoization](#memoization-6)
  - [All construct](#all-construct)
    - [Brute force](#brute-force-7)
    - [Memoization](#memoization-7)
  - [TODO Tabulation](#todo-tabulation)
- [Top 20 Dynamic Programming Interview Questions](#top-20-dynamic-programming-interview-questions)
  - [Longest Common Subsequence](#longest-common-subsequence)
- [Graph algorithms](#graph-algorithms)
  - [Depth first print](#depth-first-print)
  - [Breadth first print](#breadth-first-print)
  - [Has path](#has-path)
  - [Undirected path](#undirected-path)
  - [Connected components count](#connected-components-count)
  - [Largest component](#largest-component)
  - [Shortest path](#shortest-path)
  - [Island count](#island-count)
  - [Minimum island](#minimum-island)
- [Linked Lists](#linked-lists)
  - [reverse list](#reverse-list)
  - [zipper lists](#zipper-lists)

# Special functions

## compare

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

## substr
* https://www.cplusplus.com/reference/string/string/substr/
* `string substr (size_t pos = 0, size_t len = npos) const;`
* pos - Position of the first character to be copied as a substring.
* len - Number of characters to include in the substring, string::npos indicates all characters until the end of the string.

## find
* https://www.cplusplus.com/reference/string/string/find/
* `size_t find (const string& str, size_t pos = 0) const;`
* pos - Position of the first character in the string to be considered in the search.
* return - The position of the first character of the first match. If no matches were found, the function returns string::npos.

# Dynamic Programming
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

The function shuold return an array containing any combination of elements that add up to exactly the targetSum. If there is no combination that adds up to the targetSum, then return null.

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

# Top 20 Dynamic Programming Interview Questions
* https://www.geeksforgeeks.org/top-20-dynamic-programming-interview-questions/

## Longest Common Subsequence
* https://www.geeksforgeeks.org/longest-common-subsequence-dp-4/
* LCS Problem Statement: Given two sequences, find the length of longest subsequence present in both of them. A subsequence is a sequence that appears in the same relative order, but not necessarily contiguous. For example, “abc”, “abg”, “bdf”, “aeg”, ‘”acefg”, .. etc are subsequences of “abcdefg”. 
* [cpp](dynamic_programming/longest_common_subsequence.cpp)
* Brute force - O((2^(n+m)))
    * Space - O(1)
* memoized = O(mn)
    * Space O(mn)

# Graph algorithms

* Graph Algorithms for Technical Interviews - Full Course
    * https://youtu.be/tWVWeAqZ0WU
    * need adjacency list

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