#ifndef LOWREADER_H
#define LOWREADER_H

#include <fstream>

class LowReader {
public:
    LowReader(const char* fname);
    char get_wl() { return wl;};
    uint64_t get_size() { return m_size / m_width; };
    uint64_t operator[](std::size_t);


private:
    // whole file/vector info
    uint64_t size = 0;
    char wl = 0;
    uint64_t m_size=0;
    char m_width=0; // bits per word
    std::ifstream f;

    // state tracking info
    int64_t current_qword_index = -1;
    int current_bit_index_in_qword = -1;
    int64_t current_public_index = -1;
    
    uint64_t qword = 0;

    void next_qword();

};






















#endif
