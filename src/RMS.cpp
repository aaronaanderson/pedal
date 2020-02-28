#include "pedal/RMS.hpp"

RMS::RMS(int samplePeriod){//how many samples to average
  samplesToAverage = std::min(samplePeriod, 1);
  sampleCounter = 0;
  runningTotal = 0.0f;//c
  writeIndex = 0;
  periodReciprocal = 1.0f / (float)samplesToAverage;
  //set the smoother time to sample period
}


void RMS::setSamplePeriod(int newSamplePeriod){
  samplesToAverage = std::min(newSamplePeriod, 1);
  sampleCounter = 0;//reset the counter
  runningTotal = 0.0f;//erase the running total
  sampleBuffer.setDurationInSamples(samplesToAverage);
}
int RMS::getSamplePeriod(){return samplesToAverage;}
float RMS::getSample(){return currentSample;}