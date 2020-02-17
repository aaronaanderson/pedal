#include "pedal/CombFilter.hpp"

//Constructors and deconstructors=====================
CombFilter::CombFilter(float maxDelay){
  delayLine.setDuration(maxDelay);
  feedBackGain = 0.7f;
  delayTime = 100.0f;
}
//Core functionality of class=========================
float CombFilter::process(float input){  
  float oldSample = delayLine.getDelayed(delayTime);
  currentSample = input + (oldSample * feedBackGain);
  delayLine.inputSample(currentSample);
  return currentSample;//report the result
}
//getters and setters================================
float CombFilter::getFeedBackDelayTime(){return delayTime;}
float CombFilter::getFeedBackGain(){return feedBackGain;}
float CombFilter::getMaxDelayTime(){return maxDelayTime;}
void CombFilter::setDelayTime(float newDelayTime){
  newDelayTime = clamp(newDelayTime, 0.0f, delayLine.getDuration());
  delayTime =  newDelayTime;
}
void CombFilter::setFeedBackGain(float newFBGain){
  feedBackGain = clamp(newFBGain, -1.0f, 1.0f);
}

void CombFilter::setDelayByFrequency(float frequency){
  //convert from frequency to period, then multiply by sampling rate
  //TODO revisit this with a fresh brain
  delayTime = (pdlSettings::sampleRate)/(frequency*M_PI_2);//delayTime in MS
}

void CombFilter::setMaxDelayTime(float newMaxDelay){//maximum available delay (don't use more than you need)
  delayLine.setDuration(newMaxDelay);
  maxDelayTime = newMaxDelay;
}