#ifndef BufferedRMS_HPP
#define BufferedRMS_HPP

#include <algorithm>
#include "utilities.hpp"
#include "Buffer.hpp"
#include "pedal/DebugTool.hpp"

//Root Mean Squared
//average adjusted accurately per sample
class BufferedRMS{
  public:
  BufferedRMS(int samplePeriod = 16);//this tool is best suited for small periods
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
  DebugTool debugTool;

};
inline float BufferedRMS::process(float input){
  writeIndex = writeIndex%samplesToAverage;//wrap the index
  //subtract what WAS in the buffer first
  runningTotal -= sampleBuffer.getSample(writeIndex);
  //debugTool.printOncePerBuffer(writeIndex);
  //add its replacement to the total, then to buffer
  float squaredInput = input * input;//square the input
  runningTotal += squaredInput;//add squared input to running total
  sampleBuffer.writeSample(squaredInput, writeIndex);//add the squared input to the buffer
  writeIndex++;//increment the index for next round
  //record the sqrt of the current average
  currentSample = sqrt(runningTotal * periodReciprocal);
  return currentSample;//return value
}
#endif