#ifndef StreamedRMS_hpp
#define StreamedRMS_HPP

#include <algorithm>
#include "utilities.hpp"

class StreamedRMS{
  public:
  StreamedRMS(int samplePeriod = 32);
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
    if(runningTotal>0.0f){
      float dB = 20.0f * std::log10(sqrt(runningTotal*periodReciprocal));
      smoothOutput.setTarget(dB);
    }else{
      smoothOutput.setTarget(0.0f);
    }
    runningTotal = 0.0f;
  }
  runningTotal += input * input;//square and accumulate the input for averaging
  sampleCounter++;
  currentSample = smoothOutput.process();;
  return currentSample;
}
#endif