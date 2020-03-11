#include "pedal/StreamedRMS.hpp"

StreamedRMS::StreamedRMS(int samplePeriod){
  samplesToAverage = std::max(samplePeriod, 1);
  //store this since used per sample
  periodReciprocal = 1.0f / (float)samplesToAverage;
  //reset system
  sampleCounter = 0;
  currentSample = 0.0f;
  runningTotal = 0.0f;
  //smoothing time is exactly the size of analysis interval
  smoothOutput.setTime(samplesToMS(samplesToAverage));
}
//StreamedRMS::process(float input) is in header, b/c inlind

void StreamedRMS::setSamplePeriod(int newSamplePeriod){
  //if >=, no need to reset counter or accumulation
  if(newSamplePeriod >= samplesToAverage){
    samplesToAverage = std::max(newSamplePeriod, 1);
    periodReciprocal = 1.0f / (float)samplesToAverage;
  }else{//must reset 
    samplesToAverage = std::max(newSamplePeriod, 1);
    //reset counter
    sampleCounter = 0;
    runningTotal = 0.0f;
    periodReciprocal = 1.0f / (float)samplesToAverage;
  }  
}
int StreamedRMS::getSamplePeriod(){return samplesToAverage;}
float StreamedRMS::getSample(){return smoothOutput.getCurrentValue();}