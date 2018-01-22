#include "LowReader.h"
#include <sdsl/sd_vector.hpp>
#include <iostream>
#include <rapidcheck.h>

#include <vector>
#include <algorithm>

bool samecheck(const sdsl::sd_vector<> &sd, LowReader* lr)
{
    for (size_t i = 0; i < sd.low.size(); i++) {
        if (sd.low[i] != (*lr)[i]) return false;
    }
    return true;

}

int main()
{

    rc::check("Serializing and streaming back an sd_vector have same /low/ Elias-Fano vector",
            [](const std::vector<bool> &l0) {
                  // auto l1 = l0;

                  // copy test vector to bit_vector
                  sdsl::bit_vector bv;
                  bv.resize(l0.size());
                  for (size_t i = 0; i < l0.size(); i++) {
                      bv[i] = l0[i];
                  }

                  // build an elias-fano encoded version
                  sdsl::sd_vector<> sd(bv);

                  std::ofstream f("rc_sd_vector.bin");
                  sd.serialize(f);
                  f.close();

                  sdsl::sd_vector<> loaded_sd;
                  load_from_file(loaded_sd, "rc_sd_vector.bin");
                  
                  LowReader* lr = new LowReader("rc_sd_vector.bin");
                  
              
              //std::reverse(begin(l1), end(l1));
              //std::reverse(begin(l1), end(l1));
                  RC_ASSERT(sd.size() == loaded_sd.size());
                  RC_ASSERT(lr->get_size() == sd.low.size());
                  RC_ASSERT(samecheck(sd, lr));
                  delete lr;
            });

    
    // LowReader l("sd_vector.bin");
    // std::cout << "wl: " << (int)l.get_wl() << std::endl;

    // for (size_t i = 0; i < l.get_size(); i++ ) {
    //     std::cout << i << " : " << l[i] << std::endl;
    // }
    

    return 0;
}
