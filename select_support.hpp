#ifndef INCLUDED_SELECTSUPPORT
#define INCLUDED_SELECTSUPPORT

#include <sdsl/bit_vectors.hpp>
#include "rank_support.hpp"

using namespace std;
using namespace sdsl;


class SelectSupport {
    public:
        bit_vector* bv;
        RankSupport* rs;

        SelectSupport();
        SelectSupport(bit_vector *b);
        ~SelectSupport();

        //Returns the position, in the underlying bit-vector, of the ith 1.
        uint64_t select1(uint64_t i);

        // Returns the size of the select data structure (in bits) that is 
        // required to support log-time select on the current bitvector.
        uint64_t overhead();

        // Saves the select data structure for this bit vector to the file fname 
        // (your bit vector should also have a save() function).
        void save(string& fname);

        // Loads the select data structure for this bit vector from the file fname 
        // (your bit vector should also have a load() function).
        void load(string& fname);
};

#endif