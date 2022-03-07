#ifndef INCLUDED_RANKSUPPORT
#define INCLUDED_RANKSUPPORT

#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;


class RankSupport {
    public:
        
    bit_vector* bv;
    int_vector<> super_blocks;
    int_vector<> blocks;

    int sb_length;
    int b_length;

    // Constructor
    RankSupport();
    RankSupport(bit_vector *b);
    ~RankSupport();
    
    
    // Returns the number of 1s in the underlying bit-vector up to position i (inclusive)
    uint64_t rank1(uint64_t i);

    // Returns the size of the rank data structure (in bits) that is required to support
    // constant-time rank on the current bitvector.
    uint64_t overhead();

    // Saves the rank data structure for this bit vector to the file fname
    // (your bit vector should also have a save() function)
    void save(string& fname);

    //  Loads the rank data structure for this bit vector from the file fname
    // (your bit vector should also have a load() function)
    void load(string& fname);
};

#endif