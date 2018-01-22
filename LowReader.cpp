#include "LowReader.h"
#include <iostream>
#include <cassert>
LowReader::LowReader(const char* fname) : f(std::ifstream(fname))
{
    
    // file header
    std::cout << "file header:" << std::endl; 
    f.read((char*)&size, sizeof(size));
    std::cout << "(8 bytes from file) size: " << size << std::endl;

    f.read((char*)&wl, sizeof(wl));
    std::cout << "(1 byte from file) wl: " <<(int)wl << std::endl << std::endl;

    // int_vector header
    f.read((char*)&m_size, sizeof(m_size));
    std::cout <<  "(8 bytes from file) m_size: " <<m_size << std::endl; //(total number of bits)

    
    f.read((char*)&m_width, sizeof(m_width));
    std::cout <<  "(1 byte from file) m_width: " <<(int)m_width << std::endl; // bits per integer

    
}


uint64_t LowReader::operator[](std::size_t i)
{
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
        uint64_t bits_needed = msbit % 64;
        mask = (1 << bits_needed) - 1;
        uint64_t upper = (qword & mask) << (m_width - bits_needed);

        return upper | lower;
    }

}

static void dump_word(uint64_t w)
{

    for (int64_t i = 63; i >= 0; i--) {
        bool bit = ((uint64_t)1<<(uint64_t)i) & w;
        std::cout << bit ;
    }
}

void LowReader::next_qword()
{
    current_qword_index++;
    f.read((char*)&qword, sizeof(qword));
    // dump_word(qword);
    // std::cout << " from file" << std::endl;
    current_bit_index_in_qword = 0;
}
