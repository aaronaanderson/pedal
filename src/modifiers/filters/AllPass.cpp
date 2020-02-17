#include "pedal/AllPass.hpp"

AllPass::AllPass(){
  delayLine.setDuration(maxDelay);
  coefficient = 0.7f;
  delayTime = 10.0f;
  previousSample = 0.0f;
}

float AllPass::filter(float input){
  delayLine.inputSample(input + previousSample);
  currentSample = delayLine.getDelayed(delayTime);
  currentSample += input * coefficient;
  previousSample = currentSample * -coefficient;
  return currentSample;
}

float AllPass::getDelayTime(){return delayTime;}
float AllPass::getMaxDelay(){return maxDelay;}
float AllPass::getCoefficient(){return coefficient;}
void AllPass::setDelayTime(float newDelayTime){
  if(newDelayTime < maxDelay){
    newDelayTime = delayTime;
  }
}
void AllPass::setMaxDelay(float newMaxDelay){
  if(newMaxDelay > 0.0f){
    delayLine.setDuration(newMaxDelay);
  }
}
void AllPass::setCoefficient(float newCoefficient){coefficient = newCoefficient;}