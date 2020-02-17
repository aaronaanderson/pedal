#ifndef AllPass_hpp
#define AllPass_hpp

#include "CircularBuffer.hpp"

//Diagram based on https://www.uaudio.com/blog/allpass-filters/
//         --->*a-----
//        /           \
//input__/___[DELAY]___\__output 
//         \            /
//          \          /
//           -*(-a)<---

class AllPass{
  public:
  AllPass();

  float filter(float input);
  
  float getDelayTime();
  float getMaxDelay();
  void setDelayTime(float newDelayTime);
  void setMaxDelay(float newMaxDelay);

  private:
  float currentSample;
  CircularBuffer delayLine;
  float delayTime;
  float maxDelay = 2000.0f;
  float coefficient;
};
#endif 