#ifndef WhiteNoise_hpp
#define WhiteNoise_hpp

#include "utilities.hpp"

class WhiteNoise{//basic white noise signal generator
  public:
  WhiteNoise();//Constructor
  ~WhiteNoise();//Deconstructor
  float generateSample();//generate a single sample
  float* generateBlock();//generate a block of samples

  float getSample();//retrieve a sample w/o advancing
  float* getBlock();//retrieve a block w/o advancing

  private:
  float currentSample;//store current sample
  float* currentBlock = nullptr;//store curent buffer 
};
#endif