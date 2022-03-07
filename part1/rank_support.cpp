#include <cstdint>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sdsl/bit_vectors.hpp>
#include "rank_support.hpp"

using namespace std;
using namespace sdsl;

RankSupport::RankSupport() {
    bv = new bit_vector(64);
    blocks = int_vector<>(16, 0, 1);
    super_blocks = int_vector<>(16, 0, 1);
}

// Constructor
RankSupport::RankSupport(bit_vector *b)
{
    bv = b;

    int bv_size = (*bv).size(); // Number of elements in the bit vector
    // cout << "Bit vector size: " << bv_size << endl;

    b_length = (int)ceil(log2(bv_size)); // Number of bits of the bit vector covered by each block
    // cout << "Block length: " << b_length << endl;
    sb_length = (int)ceil(pow(b_length, 2)); // Number of bits of the bit vector covered by each superblock
    // cout << "Superblock length: " << sb_length << endl;

    int b_size = (int)ceil(bv_size * 1.0 / b_length); // Number of blocks
    // cout << "Block size: " << b_size << endl;
    int sb_size = (int)ceil(bv_size * 1.0 / sb_length); // Number of superblocks
    // cout << "Superblock size: " << sb_size << endl;

    int b_width = (int)ceil(log2(sb_length)); // Number of bits needed to store a block
    // cout << "Block width: " << b_width << endl;
    int sb_width = (int)ceil(log2(bv_size)); // Number of bits needed to store a superblock
    // cout << "Superblock width: " << sb_width << endl;

    blocks = int_vector<>(b_size, 0, b_width);
    super_blocks = int_vector<>(sb_size, 0, sb_width);

    int b_per_sb = (int)ceil(log2(bv_size));
    // cout << "Blocks per superblock: " << b_per_sb << endl;

    // cout << *bv << endl;

    for (int i = 0; i < bv_size; i++)
    {
        int sb_index = floor(i / sb_length) + 1;
        int b_index = floor(i / b_length) + 1;
        bool new_block_chunk = b_index % b_length == 0; // First block of a superblock?

        // cout << i << " " << sb_index << " " << b_index << endl;

        if (i % sb_length == 0)
        {
            super_blocks[sb_index] = super_blocks[sb_index - 1];
        }
        if (i % b_length == 0 && !new_block_chunk)
        {
            blocks[b_index] = blocks[b_index - 1];
        }

        if ((*bv)[i] == 1)
        {
            super_blocks[sb_index] += 1;
            if (!new_block_chunk)
            {
                blocks[b_index] += 1;
            }
        }
    }
    // cout << super_blocks << endl;
    // cout << blocks << endl;    
}

RankSupport::~RankSupport() {}

// Returns the number of 1s in the underlying bit-vector up to position i (inclusive)
uint64_t RankSupport::rank1(uint64_t i) {
    int sb_index = floor(i / sb_length);
    int b_index = floor(i / b_length);
    int sb_val = super_blocks[sb_index];
    int b_val = blocks[b_index];

    // cout << sb_index << " " << b_index << " " << sb_val << " " << b_val << endl;

    uint64_t total_rank = sb_val + b_val;
    
    int extra_bits = i % b_length;
    int bv_start_extra_index = i - extra_bits;
    uint64_t temp = (*bv).get_int(bv_start_extra_index, extra_bits+1);
    total_rank += __builtin_popcount(temp);
    // cout << total_rank << endl;
    return total_rank;
}

// Returns the size of the rank data structure (in bits) that is required to support
// constant-time rank on the current bitvector.
uint64_t RankSupport::overhead() {
    return (uint64_t) super_blocks.width() * super_blocks.size() + blocks.width() * blocks.size();
}

// Saves the rank data structure for this bit vector to the file fname
// (your bit vector should also have a save() function)
void RankSupport::save(string& fname) {
    ofstream file_obj;
    file_obj.open(fname);
    bv->serialize(file_obj);
    super_blocks.serialize(file_obj);
    blocks.serialize(file_obj);
    file_obj.close();
}

//  Loads the rank data structure for this bit vector from the file fname
// (your bit vector should also have a load() function)
void RankSupport::load(string& fname) {
    ifstream file_obj;
    file_obj.open(fname);
    bv->load(file_obj);
    super_blocks.load(file_obj);
    blocks.load(file_obj);
    file_obj.close();

    b_length = (int)ceil(log2(bv->size())); 
    sb_length = (int)ceil(pow(b_length, 2));
}

