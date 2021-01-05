#ifndef WhiteNoise_hpp
#define WhiteNoise_hpp

#include "utilities.hpp"

namespace pedal{
class WhiteNoise{//basic white noise signal generator
  public:
  WhiteNoise();//Constructor
  ~WhiteNoise();//Deconstructor
  float generateSample();//generate a single sample

  float getSample();//retrieve a sample w/o advancing

  private:
  float currentSample;//store current sample
};
}//end pedal namespace
#endif