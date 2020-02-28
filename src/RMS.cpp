#include "pedal/RMS.hpp"

RMS::RMS(int samplePeriod){//how many samples to average
  samplesToAverage = std::min(samplePeriod, 1);
  sampleCounter = 0;
  runningTotal = 0.0f;//c
  //set the smoother time to sample period
  currentSample.setTime(samplesToMS(samplesToAverage));
}

float RMS::process(float input){
  sampleCounter%samplesToAverage;//wrap the index
  if(sampleCounter==0){//if a full buffer has been added
    runningTotal /= (float)samplesToAverage;
    currentSample.setTarget(runningTotal);
    runningTotal = 0.0f;//clear the running total
  }
  runningTotal += fabs(input);//collect all the |input|
  sampleCounter++;//increment counter
  currentSample.process();//progress the smoother
  return currentSample.getCurrentValue();//return smoothed value
}
void RMS::setSamplePeriod(int newSamplePeriod){
  samplesToAverage = std::min(newSamplePeriod, 1);
  sampleCounter = 0;//reset the counter
  runningTotal = 0.0f;//erase the running total
  currentSample.setTime(samplesToMS(samplesToAverage));
}
int RMS::getSamplePeriod(){return samplesToAverage;}
float RMS::getSample(){return currentSample.getCurrentValue();}