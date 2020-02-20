#ifndef AllPass_hpp
#define AllPass_hpp

#include "CircularBuffer.hpp"

// Diagram from The Audio Programming Book, page 495
// input__+_______+_*-g__output
//        |      |    |
//        |   Delay   |
//        |_*g_<_|_>__|
//          
class AllPass{
  public:
  AllPass();

  float process(float input);
  
  float getDelayTime();
  float getMaxDelay();
  float getCoefficient();
  void setDelayTime(float newDelayTime);
  void setMaxDelayTime(float newMaxDelay);
  void setCoefficient(float newCoefficient);
  
  private:
  float currentSample;
  CircularBuffer delayLine;
  float delayTime;
  float maxDelay = 2000.0f;
  float coefficient;
};
#endif 