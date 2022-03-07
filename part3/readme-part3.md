# Homework 1 Part 3

## Build instructions

Navigate to the main directory `CMSC585D-Sp22-HW1`.

Compile:
```g++ -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib part3/part3.cpp -o part3/part3 part1/rank_support.cpp part3/sparse_array.cpp -lsdsl -ldivsufsort -ldivsufsort64```

Run:
```./part3/part3```

## Implementation description

The SparseArray class represents a sparse array using rank support on a bit vector representing the element positions and a dense vector of the elements themselves. It stores a bit_vector, a RankSupport data structure built on the bit_vector, and a vector of strings.

### Method descriptions

* `void create(uint64_t size)`: Creates a new bit_vector with the given size
* `bool append(string elem, uint64_t pos)`: Appends elem to the element vector and sets the bit vector as pos to 1 and returns true. If `pos` is greater than the bit vector size or if that position is already 1, it does nothing and returns false.
* `void build_rank_support()`: Builds a new rank support data structure on the current state of the bit vector.
* `bool get_at_rank(uint64_t r, string& elem)`: Sets `elem` to the the `r`th element in the element vector and returns true. If there are fewer than `r` elements, returns false.
* `bool get_at_index(uint64_t r, string& elem)`: Uses the rank support data structure to determine the index in the elements vector that corresponds to the index `r` in the bit vector. Sets `elem` to the element at that index and returns true. If `r` is larger than the bit vector size or if the bit vector does not have an element at `r`, returns false.
* `uint64_t num_elem_at(uint64_t r)`: Returns the rank of index `r`.
* `uint64_t size()`: Returns the size of the bit vector.
* `uint64_t num_elem()`: Returns the size of the elements vector.
* `void save(string& fname)`: Saves the underlying rank support data structure and element vector. This does not directly save the bit vector because the rank support data structure will save the bit vector.
* `void load(string& fname)`: Loads the underlying rank support data structure, sets the bit vector to its bit vector, and loads the elements vector.


Notes:
* I changed the return type of `append` from `void` to `bool` to indicate whether or not the function was able to insert the element. It may fail to insert if `pos` is larger than the bit vector or if an element already exists at that position. 
* I added a new function `build_rank_support`. This method should be called after all elements are inserted in order to be able to use the other methods. This keeps the structure from rebuilding the rank support data structure either after every insertion or before every method call.