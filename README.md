# Dynamic Programming
* https://youtu.be/oBt53YbR9Kk

## Fibonacci
Write a function fib(n) that takes in a number as an argument. The function should return the n-th number of the Fibonacci sequence.

The 1st and 2nd number of the sequence is 1. To generare the next number of the sequence, we sum the previous two.

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

### Brute force
O(2^(n+m)) time complexity , n+m is the height of the tree, we split by 2 (right or down)
O(n+m) space complexity

### Memoization
O(m*n) -> O(n) time complexity
O(n+m) space complexity