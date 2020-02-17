#ifndef AllPass_hpp
#define AllPass_hpp

#include "CircularBuffer.hpp"

//Diagram based on https://www.uaudio.com/blog/allpass-filters/ fig 4
//         --->*a-----
//        /           \
//input__/___[DELAY]___\__output 
//         \            /
//          \          /
//           -*(-a)<---

// Diagram from The Audio Programming Book, page 495
// input__+_______+_*-g__output
//        |     |     |
//        |   Delay   |
//        |_*g_<_|_>__|
//          
class AllPass{
  public:
  AllPass();

  float filter(float input);
  
  float getDelayTime();
  float getMaxDelay();
  float getCoefficient();
  void setDelayTime(float newDelayTime);
  void setMaxDelay(float newMaxDelay);
  void setCoefficient(float newCoefficient);
  
  private:
  float currentSample;
  CircularBuffer delayLine;
  float delayTime;
  float maxDelay = 2000.0f;
  float coefficient;
};
#endif 