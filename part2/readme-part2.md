# Homework 1 Part 2

## Build instructions

From the main directory: `CMSC585D-Sp22-HW1`.

Example compilation command:
```g++ -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib part2/part2.cpp -o part2/part2 part1/rank_support.cpp part2/select_support.cpp -lsdsl -ldivsufsort -ldivsufsort64```

Example run command:
```./part2/part2```


## Implementation description

The select support data structure keeps a pointer to the bit vector and a rank support data structure. In order to calculate the select operation, it performs a binary search of the bit vector using the `rank1` method of the rank support object.
