# Homework 1 Part 1

## Build instructions

Example compilation command:
```g++ -std=c++11 -O3 -DNDEBUG -I ~/include -L ~/lib part1/part1.cpp -o part1/part1 part1/rank_support.cpp -lsdsl -ldivsufsort -ldivsufsort64```

Example run command:
```./part1/part1```


## Implementation description

In order to implement rank support for a bit vector, I used the blocks and superblocks implementation introduced by (citation). The sizes (number of elements), lengths (number of bits in the bit vector covered), and widths (number of bits needed to store each element) of each component vector as follows:

|           |Length $l$|Size|Width|
|-----------|---|---|---|
|Bit vector |  $1$ | $n$ | $1$ |
|Superblocks| $(l_b)^2=\lceil\log_2(n)\rceil^2$ |$\frac{n}{l_{sb}} = \frac{n}{\lceil\log_2(n)\rceil^2}$ | $\lceil\log_2n\rceil$|
|Blocks     | $\lceil\log_2(n)\rceil$ | $\frac{n}{l_b}=\frac{n}{\lceil\log_2(n)\rceil}$|$\log_2l_{sb} = \lceil\log_2\lceil\log_2n\rceil^2\rceil$|

The blocks and superblocks are filled in by doing a single loop over the bit vector and calculating the relavant indices. If it is the first index of a superblock, it will copy the value from the previous superblock. If is is the first index of a block, but not the first block of a super block, it copies the value from the previous super block. Then it will increase the values if the bit vector at that index is 1 (unless it is the first block in a superblock, which stays 0).

To perform the `rank1` operation, I calculate the superblock index as $\lfloor \frac{i}{l_{sb}}\rfloor$ and the block index as $\lfloor \frac{i}{l_b}\rfloor$ and I sum the values at these indices. I calculate the bits not accounted for in the superblocks and blocks as $i \% l_b$.  I then get the integer value associated with the bit vector from indices $i$ - number of extra bits to i (inclusive). To get the number of bits in that integer I use `popcount`, and I add this value to the previous sum for the final rank.

I calculate the overhead of the data structure as the width of the superblocks * the number of superblocks + the width of the blocks * the number of blocks. It should be noted that this does not take into account the storage details of the `sdsl` int_vectors or the bit_vector itself and will not vary between bit vectors of different sizes regardless of their sparsity.
