#include <cstdint>
#include <string>
#include <sdsl/bit_vectors.hpp>
#include "sparse_array.hpp"

using namespace std;

// Creates an empty sparse array of length size 
// (the size of the underlying bitvector you will create).
void SparseArray::create(uint64_t size) {
    bv = new bit_vector(size);
}

// Appends the element elem at index pos of the sparse array. 
// You may assume this is the only way to insert elements, 
// and so you will always insert element in-order and never 
// will insert two element with the same position, further 
// you may assume that you will always have pos < size 
// (but you should probably guard against this anyway).
bool SparseArray::append(string elem, uint64_t pos) {
    if (pos > bv->size() || (*bv)[pos] == 1) {
        return false;
    }

    elements.push_back(elem);
    (*bv)[pos] = 1;
    return true;
}

void SparseArray::build_rank_support() {
    rs = new RankSupport(bv);
}

// This function places a reference to the r-th present item in the 
// array in the reference elem. It returns true if there was >= r items 
// in the sparse array and false otherwise.
bool SparseArray::get_at_rank(uint64_t r, string& elem) {
    if (r > elements.size()) return false;
    elem = elements[r];
    return true;
}

// This function looks at the r-th index in the sparse bitvector; 
// if that bit is 1, it fetches the corresponding value and binds it 
// to the reference elem and returns true, if that bit is a 0, 
// it simply returns false.
bool SparseArray::get_at_index(uint64_t r, string& elem) {
    // cout << *((*rs).bv) << endl;
    if (r > bv->size()) return false;
    int bv_val = (*bv)[r];
    // cout << "bv_val: " << bv_val << endl;
    if (bv_val == 1) {
        uint64_t rank = rs->rank1(r);
        // cout << "rank: " << rank << endl;
        elem = elements[rank-1];
        return true;
    }
    return false;
}

// This function returns the count of present elements (1s in the bit vector) 
// up to and including index r.
uint64_t SparseArray::num_elem_at(uint64_t r) {
    return rs->rank1(r);
}

// Returns the size of the sparse array.
uint64_t SparseArray::size() {
    return bv->size();
}

// Returns the number of present elements in the sparse array (i.e. the number of 1s in the bitvector).
uint64_t SparseArray::num_elem() {
    return elements.size();
}

// Saves the sparse array to the file fname.
void SparseArray::save(string& fname) {
    rs->save(fname);
    
}

// Loads the sparse array from the file fname.
void SparseArray::load(string& fname) {
    rs->load(fname);
    bv=(*rs).bv;
}
