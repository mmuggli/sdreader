#include "LowReader.h"
#include "HighReader.h"
#include <sdsl/sd_vector.hpp>
#include <iostream>
#include <rapidcheck.h>

#include <vector>
#include <algorithm>

bool samelowcheck(const sdsl::sd_vector<> &sd, LowReader* lr)
{
    for (size_t i = 0; i < sd.low.size(); i++) {
        if (sd.low[i] != (*lr)[i]) return false;
    }
    return true;

}

bool samehighcheck(const sdsl::sd_vector<> &sd, HighReader* hr)
{
    for (size_t i = 0; i < sd.high.size(); i++) {
        if (sd.high[i] != (*hr)[i]) return false;
    }
    return true;

}

int main()
{
    //if (false)
    rc::check("bools: Serializing and streaming back an sd_vector have same Elias-Fano vector components",
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

                  // write it to disk
                  std::ofstream f("rc_sd_vector.bin");
                  sd.serialize(f);
                  f.close();

                  // read a copy from disk in case we want to verify sdsl-lite actually reads/write a consistent thing
                  sdsl::sd_vector<> loaded_sd;
                  load_from_file(loaded_sd, "rc_sd_vector.bin");
                  
                  LowReader* lr = new LowReader("rc_sd_vector.bin");
                  HighReader* hr = new HighReader("rc_sd_vector.bin");
              
              //std::reverse(begin(l1), end(l1));
              //std::reverse(begin(l1), end(l1));
                  
                  // check they (the low parts) are the same size
                  RC_ASSERT(lr->get_size() == sd.low.size());
                  RC_ASSERT(hr->get_size() == sd.high.size());
                            

                  // check they have the same values
                  RC_ASSERT(samelowcheck(sd, lr));
                  RC_ASSERT(samehighcheck(sd, hr));          
                  delete lr;
                  delete hr;
            });

//    if (false)
    rc::check("ints: Serializing and streaming back an sd_vector have same Elias-Fano vector components",
            [](const std::vector<uint16_t> &l0) {
                  //RC_PRE(l0.size() >= 3);
                      auto l1 = l0;
                      std::sort(l1.begin(), l1.end());
                      std::vector<uint16_t>::iterator it = std::unique(l1.begin(), l1.end());
                      l1.resize(std::distance(l1.begin(), it));

                      RC_PRE(l1.size() >= 1);
                      // copy test vector to bit_vector
                      sdsl::bit_vector bv;
                      bv.resize(*(l1.end()-1));
                      sdsl::util::set_to_value(bv, 0);
                      
                      for (size_t i = 0; i < l1.size() - 1; i++) {
                          bv[l1[i]] = true;
                      }

                      // build an elias-fano encoded version
                      sdsl::sd_vector<> sd(bv);

                      // write it to disk
                      std::ofstream f("rc_sd_vector.bin");
                      sd.serialize(f);
                      f.close();

                      // read a copy from disk in case we want to verify sdsl-lite actually reads/write a consistent thing
                      sdsl::sd_vector<> loaded_sd;
                      load_from_file(loaded_sd, "rc_sd_vector.bin");
                  
                      LowReader* lr = new LowReader("rc_sd_vector.bin");
                      HighReader* hr = new HighReader("rc_sd_vector.bin");
              
                      //std::reverse(begin(l1), end(l1));
                      //std::reverse(begin(l1), end(l1));
                  
                      // check they (the low parts) are the same size
                      RC_ASSERT(lr->get_size() == sd.low.size());
                      RC_ASSERT(hr->get_size() == sd.high.size());
                                
                      // check they have the same values
                      RC_ASSERT(samelowcheck(sd, lr));
                      RC_ASSERT(samehighcheck(sd, hr));         
                      delete lr;
                      delete hr;

                      RC_ASSERT(true);

            });

    std::cout << "----- manual testing -------" << std::endl;
    //sdsl::bit_vector bv = {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,1};
    sdsl::bit_vector bv = {1,1,1,1,1,1,1,1,1,0};
    //bv.resize(256);
    
    //sdsl::util::set_to_value(bv, 0);
    // for(int i = 17; i < 256; i++) {
    //     bv[i] = 0;
    // }
    sdsl::sd_vector<> sd(bv);
    std::ofstream f("msd_vector.bin");
    sd.serialize(f);
    f.close();

     LowReader l("msd_vector.bin");
     std::cout << "wl: " << (int)l.get_wl() << std::endl;
     std::cout << "low: " << sd.low << std::endl;
    for (size_t i = 0; i < l.get_size(); i++ ) {
        std::cout << i << " : " << l[i] << std::endl;
    }
    HighReader h("msd_vector.bin");
    std::cout << "high read, orig sizes: " << h.get_size() << ", " << sd.high.size() << std::endl;
    //    std::cout << "samelowcheck = " << samelowcheck(sd, &l) << std::endl;
    

    return 0;
}
