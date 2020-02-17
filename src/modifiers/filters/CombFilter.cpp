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
float CombFilter::getMaxDelayTime(){return maxDelayTime;}
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
void CombFilter::setFBDelayByFrequency(float frequency, bool peak){
  //convert from frequency to period, then multiply by sampling rate
  feedBackDelayTime = pdlSettings::sampleRate/frequency;
  if(!peak){//if the user isn't setting this as a peak frequency
    if(feedBackGain > 0.0f){//if feedbackgain is > 0 (associates peak)
      feedBackGain *= -1.0f;//make it negative (resulting in trough at frequency)
    }
  }
}
void CombFilter::setFFDelayByFrequency(float frequency, bool peak){
  //convert from frequency to period, then multiply by sampling rate
  feedForwardDelayTime = pdlSettings::sampleRate/frequency;
  if(!peak){//if the user isn't setting this as a peak frequency
    if(feedForwardGain > 0.0f){//if feedbackgain is > 0 (associates peak)
      feedForwardGain *= -1.0f;//make it negative (resulting in trough at frequency)
    }
  }
}
void CombFilter::setMaxDelayTime(float newMaxDelay){//maximum available delay (don't use more than you need)
  inputDelayBuffer.setDuration(newMaxDelay);
  outputDelayBuffer.setDuration(newMaxDelay);
  maxDelayTime = newMaxDelay;
}