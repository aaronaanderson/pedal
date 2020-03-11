#ifndef HighPass_hpp
#define HighPass_hpp

#include "math.h"
#include "pdlSettings.hpp"
#include "iostream"

class HighPass{//One pole low pass filter
  public:
  HighPass(float frequency = 1000.0f);//default constructor
  inline float process(float input);//per sample function
  float getSample();//simple retrieval w/o processing
  void setFrequency(float newFrequency);//calculates new coefficients
  private:
  float currentSample;//storage for retrieval, and previous sample
  float frequency;//cutoff frequency
  float a, b;//coefficients
};

inline float HighPass::process(float input){
  //note that currentSample on the right side
  //of this expression is leftover from last call
  //It is effectively 'previousSample'
  currentSample = (a*input) - (b*currentSample);
  return currentSample;
}
#endif