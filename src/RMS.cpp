#include "pedal/RMS.hpp"

RMS::RMS(int samplePeriod){//how many samples to average
  samplesToAverage = fmin(samplePeriod, 1);
  sampleCounter = 0;
  runningTotal = 0.0f;//c
  //set the smoother time to sample period
  currentSample.setTime(samplesToMS(samplesToAverage));
}

float RMS::process(float input){
  sampleCounter%samplesToAverage;
  if(sampleCounter==0){//if a full buffer has been added
    runningTotal /= (float)samplesToAverage;
    currentSample.setTarget(runningTotal);
    runningTotal = 0.0f;//clear the running total
  }
  runningTotal += fabs(input);//collect all the |input|
  sampleCounter++;//increment counter
  currentSample.process();//progress the smoother
  return currentSample.getCurrentValue();
}
float RMS::getSample(){return currentSample.getCurrentValue();}