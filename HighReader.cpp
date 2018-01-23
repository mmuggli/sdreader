#include "HighReader.h"
#include <iostream>
#include <cassert>
#include <vector>

void skip_low(std::ifstream &f)
{
    uint64_t lm_size=0; // local m_size
    f.read((char*)&lm_size, sizeof(lm_size));
    std::cout <<  "skip_low: (8 bytes from file) m_size: " <<lm_size << std::endl; //(total number of bits)

    
    char lm_width=0;
    f.read((char*)&lm_width, sizeof(lm_width));
    std::cout <<  "skip_low: (1 byte from file) m_width: " <<(int)lm_width << std::endl; // bits per integer

    // files are written in blocks of 64 bytes
    // int sixtyfour  = sizeof(uint64_t)*8;


    // uint64_t num_full_blocks = lm_size / sizeof(uint64_t);
    // for (int block_num = 0; block_num < num_full_blocks; block_num++) {
    //     uint64_t t;
    //     f.read((char*)&t, sizeof(uint64_t));
    // }
    
    // if (lm_size % sizeof(uint64_t) != 0) {
    //     uint64_t t;
    //     f.read((char*)&t, sizeof(uint64_t));
    // }
    // uint64_t skipover_payload = num_full_blocks*sizeof(uint64_t) + (lm_size % sizeof(uint64_t) != 0) * sizeof(uint64_t);


    //following code is taken from sdsl-lite library, therefor this code base is the same license as sdsl-lite as it is a funky derivative of such
    // FIXME: change this whole function to just use the appropriate seek() variant rather than lots of read() calls

    // changed capacity() -> lm_size

//    sdsl::bit_vector::bit_resize(lm_size);
    size_t capacity = ((lm_size+63)>>6)<<6;

    std::vector<uint64_t> data;
    const int blocksize = 1<<22;
    data.resize(blocksize);
    uint64_t* p = data.data();
    size_t idx = 0;
    while (idx+blocksize < (capacity>>6)) {
        f.read((char*) p, blocksize*sizeof(uint64_t));
        //p     += blocksize;
        idx += blocksize;
    }
    f.read((char*) p, ((capacity>>6)-idx)*sizeof(uint64_t));
    

    
//    std::cout << "skip_low: read bytes from file = " << skipover_payload << std::endl;
}

static void dump_word(uint64_t w)
{

    for (int64_t i = 63; i >= 0; i--) {
        bool bit = ((uint64_t)1<<(uint64_t)i) & w;
        std::cout << bit ;
    }
}


HighReader::HighReader(const char* fname) : f(std::ifstream(fname))
{
    
    // file header
    std::cout << "file header:" << std::endl; 
    f.read((char*)&size, sizeof(size));
    std::cout << "(8 bytes from file) size: " << size << std::endl;

    f.read((char*)&wl, sizeof(wl));
    std::cout << "(1 byte from file) wl: " <<(int)wl << std::endl << std::endl;

    skip_low(f);
    
    // int_vector header
    f.read((char*)&m_size, sizeof(m_size));
    std::cout <<  "(8 bytes from file) m_size: " <<m_size << std::endl; //(total number of bits)

    
    // f.read((char*)&m_width, sizeof(m_width));
    // std::cout <<  "(1 byte from file) m_width: " <<(int)m_width << std::endl; // bits per integer
    m_width = 1;

    
}


uint64_t HighReader::operator[](std::size_t i)
{
    if (i == last_index) {
        return last_value;
    }
    
    if ((int64_t)i < current_public_index) {
        assert(!"Rewind not supported");
    }

    current_public_index = i;
    
    // whole word fits in qword case
    // assumes word is smaller than qword

    // compute rightmost bit
    uint64_t lsbit = i * m_width;
    uint64_t lsbit_qword = lsbit / 64;

    uint64_t msbit = lsbit + (m_width - 1);
    uint64_t msbit_qword = msbit / 64;

    while (lsbit_qword != current_qword_index) {
        next_qword();
    }

    if (lsbit_qword == msbit_qword) {
        uint64_t depth_into_word = lsbit % 64;
        uint64_t maskbits = (1 << m_width) - 1;
        uint64_t mask =  maskbits << depth_into_word;
        return (qword & mask) >> depth_into_word;
    } else {

        // lsb portion
        uint64_t depth_into_word = lsbit % 64;
        uint64_t maskbits = (1 << m_width) - 1;
        uint64_t mask =  maskbits << depth_into_word;
        uint64_t lower =  (qword & mask) >> depth_into_word;

        next_qword();
        // msb portion
        uint64_t bits_needed = (1+msbit) % 64;
        //std::cout << "bitsneeded: " << bits_needed << std::endl;
        mask = (1 << bits_needed) - 1;
        // std::cout << "mask: " ;
        // dump_word(mask);
        // std::cout << std::endl;
        uint64_t upper = (qword & mask) << (m_width - bits_needed);

        last_index = i;
        last_value = upper | lower;
        return last_value;
    }

}


void HighReader::next_qword()
{
    current_qword_index++;
    f.read((char*)&qword, sizeof(qword));
    //dump_word(qword);
     //std::cout << " from file" << std::endl;
    current_bit_index_in_qword = 0;
}
