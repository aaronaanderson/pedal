#include "pedal/Compressor.hpp"

Compressor::Compressor(){
  delayLine.setDuration(100.0f);//won't need much
  setThresholdDB(-12.0f);
  setRatio(6.0f);
  setInputGainDB(3.0f);
  setAttackTime(20.0f);
  setReleaseTime(100.0f);
  setAnalysisTime(samplesToMS(16));
}

float Compressor::process(float input){
  delayLine.inputSample(input);//feed the delay line
  //Find the amplitude in dB of the input signal
  float currentEstimate = signalEstimator.process(input);//store the estimate
  debug.printOncePerBuffer(threshold);
  if(currentEstimate > threshold){//if the threshold has been exceded 
    attackFlag = true;//
    //how many dB past the threshold was it? How many dB over?
    float differenceInDB = amplitudeToDB(currentEstimate/threshold);
    //how much should the signal be scaled by?
    float dBDown = differenceInDB * ratio * -1.0f;
    linearGain.setTarget(dBDown);
    //std::cout << dBDown << std::endl;
  }
  if(attackFlag){//if in attack phase
    attackPhaseInSamples++;//increment the attackPhase counter
    linearGain.process();//progress the smooth gain value
    if(attackPhaseInSamples >= attackTimeInSamples){//if we have reached the end of the attack phase
      attackPhaseInSamples = 0;//reset the attackPhase counter
      linearGain.setTarget(1.0f);//set target for release
      attackFlag = false;//change to release state
    }
  }else{//if in release phase
    //no need to reset target here since set to 1.0f at the end of the attack phase
    linearGain.process();//increment the smooth gain value
  }
  //the output is the delayed input scaled by this linear gain
  currentSample = delayLine.getDelayed(analysisTime) * linearGain.getCurrentValue();
  return currentSample;//return the result
}
void Compressor::setRatio(float newRatio){ratio = std::fmax(newRatio, 1.0);}
void Compressor::setInputGainDB(float newInputGainDB){linearInputGain = dBToAmplitude(newInputGainDB);}
void Compressor::setAttackTime(float newAttackTime){
  linearGain.setTimeUp(newAttackTime);
  attackTimeInSamples = msToSamples(newAttackTime);//adjust the attack phase counter
}
void Compressor::setReleaseTime(float newReleaseTime){linearGain.setTimeDown(newReleaseTime);}
void Compressor::setThresholdDB(float thresholdDB){threshold = dBToAmplitude(thresholdDB);}
void Compressor::setAnalysisTime(float newAnalysisTime){
  signalEstimator.setSamplePeriod(samplesToMS(newAnalysisTime));
}
float Compressor::getSample(){return currentSample;}
float Compressor::getThresholdDB(){return amplitudeToDB(threshold);}
float Compressor::getRatio(){return ratio;}
float Compressor::getInputGainDB(){return amplitudeToDB(linearInputGain);}
float Compressor::getAttackTime(){return linearGain.getTimeUp();}
float Compressor::getReleaseTime(){return linearGain.getTimeDown();}
