#ifndef StreamedRMS_hpp
#define StreamedRMS_HPP

#include <algorithm>
#include "utilities.hpp"

class StreamedRMS{
  public:
  StreamedRMS(int samplePeriod = 128);
  inline float process(float input);
  void setSamplePeriod(int newSamplePeriod);//how many samples to average
  int getSamplePeriod();
  float getSample();

  private:
  float periodReciprocal;//store
  float currentSample;//output only calculated every N samples, so smooth
  int samplesToAverage;//how many samples in an average?
  int sampleCounter;//to keep track of how many we've added
  float runningTotal;//keep a running total
  SmoothValue<float> smoothOutput;//determine values between analysis
};

inline float StreamedRMS::process(float input){
  sampleCounter = sampleCounter % samplesToAverage;
  if(sampleCounter == 0){//if the sampleCounter reached max
    //calculate the average value over those N samples
    smoothOutput.setTarget(runningTotal * periodReciprocal);
    runningTotal = 0.0f;
  }
  //add the abs(input). We're concerned with distance from 0.0f only
  runningTotal += std::fabs(input);
  sampleCounter++;
  currentSample = smoothOutput.getCurrentValue();
  return currentSample;
}
#endif