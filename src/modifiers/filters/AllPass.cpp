#include "pedal/AllPass.hpp"

AllPass::AllPass(){
  delayLine.setDuration(maxDelay);
  coefficient = 0.7f;
  delayTime = 10.0f;
}
float AllPass::process(float input){
  float delayedSample = delayLine.getDelayed(delayTime);
  currentSample = input + (delayedSample * coefficient);
  delayLine.inputSample(currentSample);//feed the modified input
  currentSample -= delayedSample * coefficient;
  return currentSample;
}
float AllPass::getDelayTime(){return delayTime;}
float AllPass::getMaxDelay(){return maxDelay;}
float AllPass::getCoefficient(){return coefficient;}
void AllPass::setDelayTime(float newDelayTime){
  if(newDelayTime < maxDelay){
    delayTime = newDelayTime;
  }
}
void AllPass::setMaxDelayTime(float newMaxDelay){
  if(newMaxDelay > 0.0f){
    delayLine.setDuration(newMaxDelay);
  }
  maxDelay = newMaxDelay;
}
void AllPass::setCoefficient(float newCoefficient){coefficient = newCoefficient;}