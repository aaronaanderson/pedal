#ifndef WhiteNoise_hpp
#define WhiteNoise_hpp

#include "utilities.hpp"

class WhiteNoise{
  public:
  WhiteNoise();
  float generateSample();
  float getSample();

  private:
  float currentSample;
};
#endif