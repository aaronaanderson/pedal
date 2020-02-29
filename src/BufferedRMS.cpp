#include "pedal/BufferedRMS.hpp"

BufferedRMS::BufferedRMS(int samplePeriod){//how many samples to average
  samplesToAverage = std::min(samplePeriod, 1);
  sampleCounter = 0;
  runningTotal = 0.0f;//c
  writeIndex = 0;
  periodReciprocal = 1.0f / (float)samplesToAverage;
  //set the smoother time to sample period
}


void BufferedRMS::setSamplePeriod(int newSamplePeriod){
  samplesToAverage = std::min(newSamplePeriod, 1);
  sampleCounter = 0;//reset the counter
  runningTotal = 0.0f;//erase the running total
  sampleBuffer.setDurationInSamples(samplesToAverage);
}
int BufferedRMS::getSamplePeriod(){return samplesToAverage;}
float BufferedRMS::getSample(){return currentSample;}