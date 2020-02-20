#ifndef LowPass_hpp
#define LowPass_hpp

#include "math.h"
#include "pdlSettings.hpp"
#include "iostream"

class LowPass{
  public:
  LowPass(float frequency = 1000.0f);
  inline float process(float input);
  float getSample();
  void setFrequency(float newFrequency);
  private:
  float currentSample;
  float frequency;//cutoff frequency
  float a, b;//coefficients
};

inline float LowPass::process(float input){
  //note that currentSample on the right side
  //of this expression is leftover from last call
  //It is effectively 'previousSample'
  currentSample = (a*input) - (b*currentSample);
}
#endif