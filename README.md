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

### Special functions

#### compare

* https://www.cplusplus.com/reference/string/string/compare/https://www.cplusplus.com/reference/string/string/compare/
* `int compare (size_t pos, size_t len, const string& str) const;` 
* str
    * Another string object, used entirely (or partially) as the comparing string.
* pos
    * Position of the first character in the compared string. If this is greater than the string length, it throws out_of_range. Note: The first character is denoted by a value of 0 (not 1).
* len
    * Length of compared string (if the string is shorter, as many characters as possible). A value of string::npos indicates all characters until the end of the string.
* return
    * 0	They compare equal

#### substr
* https://www.cplusplus.com/reference/string/string/substr/

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

The function should return a 2D array containing all of the ways that the target can be constructed by concatenating elements of the wordBank array. Each element of the 2D array shold represent one combination that constructs the target.

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

# Graph algorithms

* Graph Algorithms for Technical Interviews - Full Course
    * https://youtu.be/tWVWeAqZ0WU
* Algorithms Course - Graph Theory Tutorial from a Google Engineer
    * https://youtu.be/09_LlHjoEiY