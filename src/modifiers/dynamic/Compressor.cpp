#include "pedal/Compressor.hpp"

Compressor::Compressor(){
  delayLine.setDuration(100.0f);//won't need much
  setThresholdDB(-12.0f);//at what intensity should the compressor start compressing?
  setRatio(4.0f);//if over threshold by 'ratio' decibels, scale down until it's only 1/ratio decibels over threshold
  setInputGainDB(0.0f);//Raising input gain is effectively the same as lowering threshold
  setAttackTime(5.0f);//experiment with very low values (0.01ms to 10ms)
  setReleaseTime(20.0f);//generall longer than attack (10 to 100ms)
  setAnalysisTime(samplesToMS(16));//analysis window size. lower values are more sensitive to input transients.
  highestAttackPhaseTarget = 0.0f;//keep track of highest target; only update if new target is higher
  linearGain.setTarget(1.0f);
}
float Compressor::process(float input){
  delayLine.inputSample(input);//feed the delay line //no input gain on delayLine sample
  //apply the input gain
  input = input * linearInputGain;
  updateGain(input);
  //the output is the delayed input scaled by this smoothed linearGain, then multiplied by makeup gain
  currentSample = delayLine.getDelayed(analysisTime + lookAhead) * 
                  linearGain.getCurrentValue() * linearMakeUpGain;
  return currentSample;//return the result
}
float Compressor::process(float input, float sideChain){//overload for sidechain input
  delayLine.inputSample(input);//delay line fed input signal still
  sideChain = sideChain * linearInputGain;//scale sidechain by linearInputGain
  updateGain(sideChain);//update final gain value with sideChain instead of input
  //the output is the delayed input scaled by this smoothed linearGain, then multiplied by makeup gain
  currentSample = delayLine.getDelayed(analysisTime + lookAhead) * 
                  linearGain.process() * linearMakeUpGain;
  return currentSample;//return the result
}
void Compressor::updateGain(float input){
  //Find the linear intensity of scaled input signal
  float currentEstimate = signalEstimator.process(input);//store the estimate
  //debug.printOncePerBuffer(currentEstimate);
  if(currentEstimate > linearThreshold){//if the threshold has been exceded 
    //This next portion is particularily expensive as it must call both
    //amplitudeToDB and dBToAmplitude. Fortunately, this is only called
    //every time a higher currentEstimate is reached.
    if(currentEstimate > highestAttackPhaseTarget){//should only occur if exceeds previous target
      attackFlag = true;//make sure compressor is in attack phase if it wasn't already
      attackPhaseInSamples = 0;//restart the attack phase
      highestAttackPhaseTarget = currentEstimate;//record it as the highest
      //how many dB past the threshold was it? How many dB over?
      float differenceInDB = amplitudeToDB(currentEstimate) - threshold;
      //how much should the signal be scaled by?
      reductionTargetDB = differenceInDB * dBDifferenceScalar;//dBDifferenceSclar = -(1.0f - (1.0f/ratio))
      linearGain.setTarget(dBToAmplitude(reductionTargetDB));//ramp to target amplitue scalar
    }
  }
  if(attackFlag){//if in attack phase
    attackPhaseInSamples++;//increment the attackPhase counter
    if(attackPhaseInSamples >= attackTimeInSamples){//if we have reached the end of the attack phase
      highestAttackPhaseTarget = 0.0f;//reset the highest for next round
      attackPhaseInSamples = 0;//reset the attackPhase counter
      linearGain.setTarget(1.0f);//set target for release
      attackFlag = false;//change to release state
    }
  }
  linearGain.process();//update linear gain
}
void Compressor::setThresholdDB(float thresholdDB){
  highestAttackPhaseTarget = 0;//previous value may no longer be relavant
  threshold = thresholdDB;//threshold is stored in decibels
  linearThreshold = dBToAmplitude(thresholdDB);//efficient to store this, since used often
}
void Compressor::setRatio(float newRatio){//for every newRatio the input is above the output, reduce output to 1/newRatio over threshold
  ratio = std::fmax(newRatio, 1.0f);//assign the value, don't allow values below 1.0f
  dBDifferenceScalar = -(1.0f - (1.0f/ratio));//calculate this in advance since used often
}
void Compressor::setInputGainDB(float newInputGainDB){linearInputGain = dBToAmplitude(newInputGainDB);}
void Compressor::setInputGainLinear(float newLinearInputGain){linearInputGain  = newLinearInputGain;}
void Compressor::setMakeUpGainDB(float newMakeUpGainDB){linearMakeUpGain = dBToAmplitude(newMakeUpGainDB);}
void Compressor::setAttackTime(float newAttackTime){
  linearGain.setTimeDown(newAttackTime);
  attackPhaseInSamples = 0;//reset the attack phase
  attackTimeInSamples = msToSamples(newAttackTime);//adjust the attack phase counter
}
void Compressor::setReleaseTime(float newReleaseTime){linearGain.setTimeUp(newReleaseTime);}
void Compressor::setLookAheadTime(float newLookAheadTime){
  lookAhead = std::fmax(newLookAheadTime, 0.0f);
}
//lower times (8-32 samples) are more transient-sensitive
void Compressor::setAnalysisTime(float newAnalysisTime){//defaults to 16 samples
  signalEstimator.setSamplePeriod(samplesToMS(newAnalysisTime));
}
float Compressor::getSample(){return currentSample;}
//getLinearScalar() may be used to compress external signals
float Compressor::getLinearScalar(){
  return linearGain.getCurrentValue();
}
float Compressor::getThresholdDB(){return threshold;}
float Compressor::getRatio(){return ratio;}
float Compressor::getInputGainDB(){return amplitudeToDB(linearInputGain);}
float Compressor::getMakeUpGainDB(){return dBToAmplitude(linearMakeUpGain);}
float Compressor::getAttackTime(){return linearGain.getTimeUp();}
float Compressor::getReleaseTime(){return linearGain.getTimeDown();}
float Compressor::getLookAheadTime(){return lookAhead;}