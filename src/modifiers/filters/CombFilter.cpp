#include "pedal/CombFilter.hpp"

//Constructors and deconstructors=====================
CombFilter::CombFilter(){
  inputDelayBuffer.setDuration(1000.0f);
  outputDelayBuffer.setDuration(1000.0f);
}
//Core functionality of class=========================
float CombFilter::filter(float input){  
//           -->[DELAY1]*a   b*[DELAY2]<--
//        /               \ /             \
//input__/_________________+_______________\__output
  inputDelayBuffer.inputSample(input);//feed input in to delay line for later
  //grab a copy the delayed input
  float feedForwardSample = inputDelayBuffer.getDelayed(feedForwardDelayTime);
  //added the delayed sample scaled by feedForwardGain ('a' in diagram)
  currentSample = input + (feedForwardSample * feedForwardGain);
  //add the delayed output, scaled by 'b', to the current sample
  currentSample += outputDelayBuffer.getDelayed(feedBackDelayTime) * feedBackGain;
  //feed that into the feedback delay before leaving
  outputDelayBuffer.inputSample(currentSample);
  return currentSample;//report the result
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
  newFBDelayTime = clamp(newFBDelayTime, 0.0f, outputDelayBuffer.getDuration());
  feedBackDelayTime =  newFBDelayTime;
}
void CombFilter::setFeedBackGain(float newFBGain){
  feedBackGain = clamp(newFBGain, -1.0f, 1.0f);
}
void CombFilter::setFeedForwardGain(float newFFGain){
  feedForwardGain = clamp(newFFGain, -1.0f, 1.0f);
}
void CombFilter::setFBDelayByFrequency(float frequency, bool peak){
  //convert from frequency to period, then multiply by sampling rate
  feedBackDelayTime = (pdlSettings::sampleRate*0.5f)/(frequency * M_PI);
  std::cout << feedBackDelayTime << std::endl;
  if(!peak){//if the user isn't setting this as a peak frequency
    if(feedBackGain > 0.0f){//if feedbackgain is > 0 (associates peak)
      feedBackGain *= -1.0f;//make it negative (resulting in trough at frequency)
    }
  }
}
void CombFilter::setFFDelayByFrequency(float frequency, bool peak){
  //convert from frequency to period, then multiply by sampling rate
  feedForwardDelayTime = (pdlSettings::sampleRate*0.5f)/(frequency * M_PI);
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