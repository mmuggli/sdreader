#include "SDIter.h"


SDIter::SDIter(const char* fname) : lr(LowReader(fname)), hr(HighReader(fname))
{

}

signed long long SDIter::peek()
{
    if (lowndx >= lr.get_size()) {
        return -1;
    }
    nexthigh = highndx;
    next_cur_high = cur_high;
    
    size_t high_increment = 0; // accumulates number of 0's
    while (hr[nexthigh] != 1) {
        high_increment++;
        nexthigh++;
    }
    nexthigh++; // advance to next one
    next_cur_high += high_increment;
    size_t low = lr[lowndx];
    
    peeked = true;
    size_t one_loc = (next_cur_high << lr.get_wl()) | low; // '1' location

    return one_loc;
    
    
    
}
                                                     
void SDIter::advance()
{
    if (!peeked) { peek();}
    lowndx++;
    highndx = nexthigh;
    cur_high = next_cur_high;
    peeked = false;
    
}
