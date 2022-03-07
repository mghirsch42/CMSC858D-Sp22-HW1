#include <cstdint>
#include <string>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <sdsl/bit_vectors.hpp>
#include "rank_support.hpp"
#include "select_support.hpp"

using namespace std;
using namespace sdsl;

SelectSupport::SelectSupport() {

}

SelectSupport::SelectSupport(bit_vector *b) {
    bv = b;
    rs = new RankSupport(bv);
    
}

SelectSupport::~SelectSupport() { }

// Returns the position, in the underlying bit-vector, of the ith 1.
uint64_t SelectSupport::select1(uint64_t i) {
    int left_index = 0;
    int right_index = bv->size()-1;
    while (right_index > left_index) {
        int curr_index = (int) floor(left_index + (right_index - left_index) / 2);
        int curr_rank = rs->rank1(curr_index);
        if (i == curr_rank) {
            return (uint64_t) curr_index;
        }
        else if (i < curr_rank) {
            right_index = curr_index;
        }
        else {
            left_index = curr_index;
        }
    }
    return -1;
    
}

// Returns the size of the select data structure (in bits) that is 
// required to support log-time select on the current bitvector.
uint64_t SelectSupport::overhead() {
    return rs->overhead();
}

// Saves the select data structure for this bit vector to the file fname 
// (your bit vector should also have a save() function).
void SelectSupport::save(string& fname) {
    ofstream file_obj;
    rs->save(fname);
}

// Loads the select data structure for this bit vector from the file fname 
// (your bit vector should also have a load() function).
void SelectSupport::load(string& fname) {
    rs->load(fname);
    bv = (*rs).bv;
}
