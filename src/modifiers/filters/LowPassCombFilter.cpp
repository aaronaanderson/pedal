#include "pedal/LowPassCombFilter.hpp"

//Constructors and deconstructors=====================
LowPassCombFilter::LowPassCombFilter(float maxDelay, float initialFrequency){
  delayLine.setDuration(maxDelay);
  filter.setFrequency(initialFrequency);
  feedBackGain = 0.95f;
  delayTime = 100.0f;
}
//Core functionality of class=========================
float LowPassCombFilter::process(float input){  
  float oldSample = delayLine.getDelayed(delayTime);
  currentSample = input + (oldSample * feedBackGain);
  currentSample = filter.process(currentSample);
  delayLine.inputSample(currentSample);
  return currentSample;//report the result
}
//getters and setters================================
float LowPassCombFilter::getFeedBackDelayTime(){return delayTime;}
float LowPassCombFilter::getFeedBackGain(){return feedBackGain;}
float LowPassCombFilter::getMaxDelayTime(){return maxDelayTime;}
float LowPassCombFilter::getFilterFrequency(){return filterFrequency;}
void LowPassCombFilter::setDelayTime(float newDelayTime){
  newDelayTime = clamp(newDelayTime, 0.0f, delayLine.getDuration());
  delayTime =  newDelayTime;
}
void LowPassCombFilter::setFeedBackGain(float newFBGain){
  feedBackGain = clamp(newFBGain, -1.0f, 1.0f);
}
void LowPassCombFilter::setDelayByFrequency(float frequency){
  //convert from frequency to period, then multiply by sampling rate
  //TODO revisit this with a fresh brain
  delayTime = (pdlSettings::sampleRate)/(frequency*M_PI_2);//delayTime in MS
}
void LowPassCombFilter::setMaxDelayTime(float newMaxDelay){//maximum available delay (don't use more than you need)
  delayLine.setDuration(newMaxDelay);
  maxDelayTime = newMaxDelay;
}
void LowPassCombFilter::setFilterFrequency(float newFrequency){
  filter.setFrequency(newFrequency);
}