#ifndef RMS_HPP
#define RMS_HPP

#include <algorithm>
#include "utilities.hpp"
#include "Buffer.hpp"
//Root Mean Squared
class RMS{
  public:
  RMS(int samplePeriod = 64);
  inline float process(float input);
  void setSamplePeriod(int newSamplePeriod);//how many samples to average
  int getSamplePeriod();
  float getSample();
  
  private:
  Buffer sampleBuffer;
  int writeIndex;
  float periodReciprocal;//store
  float currentSample;//output only calculated every N samples, so smooth
  int samplesToAverage;//how many samples in an average?
  int sampleCounter;//to keep track of how many we've added
  float runningTotal;
};

inline float RMS::process(float input){
  sampleCounter%samplesToAverage;//wrap the index
  //subtract what WAS in the buffer first
  runningTotal -= sampleBuffer.getSample(writeIndex);
  //add its replacement to the total, then to buffer
  input = fabs(input);//we want distance from 0
  runningTotal += input;//add the input
  sampleBuffer.writeSample(input, writeIndex);
  writeIndex++;
  currentSample = runningTotal * periodReciprocal;
  return currentSample;//return smoothed value
}
#endif