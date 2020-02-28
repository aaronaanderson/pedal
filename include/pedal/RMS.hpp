#ifndef RMS_HPP
#define RMS_HPP

#include <algorithm>
#include "utilities.hpp"
//Root Mean Squared
class RMS{
  public:
  RMS(int samplePeriod = 64);
  float process(float input);
  void setSamplePeriod(int newSamplePeriod);//how many samples to average
  int getSamplePeriod();
  float getSample();
  
  private:
  SmoothValue<float> currentSample;//output only calculated every N samples, so smooth
  int samplesToAverage;//how many samples in an average?
  int sampleCounter;//to keep track of how many we've added
  float runningTotal;
};
#endif