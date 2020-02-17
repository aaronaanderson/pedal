#include "pedal/CombFilter.hpp"

//Constructors and deconstructors=====================
CombFilter::CombFilter(){
  inputDelayBuffer.setDuration(1000.0f);
  outputDelayBuffer.setDuration(1000.0f);
}

//Core functionality of class=========================
float CombFilter::filter(float input){
  inputDelayBuffer.inputSample(input);
  float feedForwardSample = inputDelayBuffer.getDelayed(feedForwardDelayTime);
  currentSample = feedForwardSample;
  currentSample += outputDelayBuffer.getDelayed(feedBackDelayTime);
  outputDelayBuffer.inputSample(currentSample);
  return currentSample;
}
//getters and setters================================
float CombFilter::getFeedForwardDelayTime(){return feedForwardDelayTime;}
float CombFilter::getFeedBackDelayTime(){return feedBackDelayTime;}
float CombFilter::getFeedBackGain(){return feedBackGain;}
float CombFilter::getFeedForwardGain(){return feedForwardGain;}
void CombFilter::setFeedForwardDelayTime(float newFFDelayTime){
  newFFDelayTime = clamp(newFFDelayTime, 0.0f, inputDelayBuffer.getDuration());
  feedForwardDelayTime = newFFDelayTime;
}
void CombFilter::setFeedBackDelayTime(float newFBDelayTime){
  newFBDelayTime = clamp(newFBDelayTime, 0.0f, outputDelayBuffer.getDuration);
  feedBackDelayTime =  newFBDelayTime;
}
void CombFilter::setFeedBackGain(float newFBGain){
  feedBackGain = clamp(newFBGain, 0.0f, 1.0f);
}
void CombFilter::setFeedForwardGain(float newFFGain){
  feedForwardGain = clamp(newFFGain, 0.0f, 1.0f);
}