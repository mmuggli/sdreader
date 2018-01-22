#ifndef SDITER_H
#define SDITER_H

#include "HighReader.h"
#include "LowReader.h"

class SDIter {
    
public:
    SDIter(const char* fname);
    signed long long peek(); 

    void advance();



/* from sd_vector, we require:
       low.size() 
       high.operator[]
       low.operator[]
       wl

     */

private:
    // position in m_s->low
    size_t lowndx = 0;

    // position in m_s->high
    // we compute a 'next' value during peek() which actually gets locked in by advance()    
    size_t highndx = 0; // points to next chunk of bits to read from m_s->high
    size_t nexthigh = 0; // if peek() has been called, points to the next chunk of bits to read from m_s->high

    // each chunk of bits in m_s->high stores a /delta/.  We need to keep track of the accumulated value of all the deltas
    // again, we compute a 'next' value during peek() which actually gets locked in by advance()
    size_t cur_high = 0; // accumulated (i.e. non-delta) value of the position of the last 1 instance
    size_t next_cur_high = 0; // if peek() has been called, accumulated (i.e. non-delta) value of the position of the peeked 1 instance
    bool peeked = false;

    LowReader lr;
    HighReader hr;


};


#endif
