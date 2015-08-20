#include <iostream>
#include <cinttypes>
#include <stdlib.h>
#include <iomanip>

/* g++ -std=gnu++11 -Wall -O3 random_seq_gen.cpp */

/*
A Linear congruential generator. This code will generate a random sequence for 
a given "seed" and "length", after skipping "skip_cnt" elements.
This can be used to genarate passwords, key-files and so on. All that user 
have to remember are "seed", "skip_cnt" and "length".
*/

/* to divide the value into two segments and swap them, to increase randomness based on seed */
//#define ENABLE_SWAP

class RandGen // 'A Custom Random Number Generator'
{
private:
   uint64_t val_;
   uint64_t a_;
   uint64_t c_;
   uint64_t mod_;
#if ENABLE_SWAP
   uint32_t lowerCut_;
   uint32_t upperCut_;
   uint32_t lowerMask_;
   uint32_t upperMask_;
#endif

public:
   RandGen(uint64_t seed){
      this->val_ = seed;
      /* using values from "newlibC" library */
      this->a_ = 6364136223846793005ULL;
      this->c_ = 1;
      //this->mod_ = 0x10000000000000000; // pow(2, 64), cannot assign as there is no data type greater than 64-bits
      
#if ENABLE_SWAP
      uint32_t unsignedMask = 0xFFFFFFFF;
      this->lowerCut_ = seed % 16;
      this->upperCut_ = 32 - this->lowerCut_;
      this->lowerMask_ = (~((0xFFFFFFFF >> this->lowerCut_)  << this->lowerCut_)) & unsignedMask;
      this->upperMask_ = (~(this->lowerMask_)) & unsignedMask;
#endif
   }

   /* generate & return a random no. */ 
   //__attribute__((always_inline)) inline
   uint32_t getNxtVal(void){
      //this->val_ = (((uint64_t)this->a_ * this->val_) + this->c_) % this->mod_;
      /* this->mod_ = pow(2, 64). No modulo operation required as the output is 64 bit */
      this->val_ = ((this->a_ * this->val_) + this->c_) ; 

#if ENABLE_SWAP
      this->val_ = ((this->val_ & this->lowerMask_) << this->upperCut_) + ((this->val_ & this->upperMask_) >> this->lowerCut_);
#endif
      return (this->val_ >> 32); // bits 63..32 to increase randomness (as done in newlibC ) 
   }
   
   /* generate the next random sequence of the given "length" */
   void getNxtSeq(uint32_t *arr, uint32_t length){
      uint64_t i;
      for (i = 0; i < length; ++i){
         arr[i] = this->getNxtVal();
      }
   }

   /* generate the next random sequence of the given "length", skipping "skip_cnt" elements */
   void getSeq(uint32_t *arr, uint64_t skip_cnt, uint32_t length){
      uint64_t i;
      
      /* skipping elements */
      for (i = 0; i < skip_cnt; ++i){
         this->getNxtVal();
      }
      
      /* generating sequence */
      this->getNxtSeq(arr, length);
   }
};

int main(int argc, char *argv[]){   
   if(argc < 4){
      std::cout << "Usage: " << argv[0] << " <seed> <start-position> <sequence-length>" << std::endl;
      return 0;
   }

   uint64_t seed = strtoull(argv[1], NULL, 10);
   uint64_t skip_cnt = strtoull(argv[2], NULL, 10);
   uint32_t seq_len = strtoul(argv[3], NULL, 10); // limited to 32-bits to avoid large output data (max 4GB)
   uint64_t i;
   uint32_t rnd_value;

   /* Initialization */
   RandGen rd = RandGen(seed);
   
   /* skipping "skip_cnt" elements */
   rd.getSeq(NULL, skip_cnt, 0);
   
   /* generate the random sequence of the given "length" */
   for(i = 0; i < seq_len; ++i){
      // get random no.
      rnd_value = rd.getNxtVal();
      //std::cout << std::setfill('0') << std::setw(8) << std::hex << rnd_value << std::endl;
      // display every random no. as charecters, LSB first
      std::cout << char(rnd_value & 0x000000FF)
                << char((rnd_value & 0x0000FF00) >> 8)
                << char((rnd_value & 0x00FF0000) >> 16)
                << char((rnd_value & 0xFF000000) >> 24);
   }
   return 0;
}
