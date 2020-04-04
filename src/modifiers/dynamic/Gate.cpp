#include "pedal/Gate.hpp"

Gate::Gate(){
  delayLine.setDuration(100.0f);//100ms max pre-delay
  setThresholdDB(-50.0f);//at what intensity should the gate start attenuating
  setRangeDB(0.0f);//how much reduction if below threshold (in addition to any reduction brought on by ratio/threshold)
  setRatio(20.0f);//if below threshold by N decibels, reduce furth to N*ratio decibels
  setInputGainDB(0.0f);//Raising input gain is effectively the same as lowering threshold
  setMakeUpGainDB(0.0f);
  setAttackTime(5.0f);//how long to reach full attenuation (in ms)
  setHoldTime(2.0f);//how long to hold attenuation before releasing
  setReleaseTime(20.0f);//how long to reach no attenuation
  setAnalysisTime(samplesToMS(64));//analysis window size. 
  linearGain.setTarget(1.0f);
}
float Gate::process(float input){
  delayLine.inputSample(input);//feed the delay line //no input gain on delayLine sample
  //apply the input gain
  input = input * linearInputGain;
  updateGain(input);
  //the output is the delayed input scaled by this smoothed linearGain, then multiplied by makeup gain
  currentSample = delayLine.getDelayed(analysisTime + lookAhead) * 
                  linearGain.getCurrentValue() * linearMakeUpGain;
  return currentSample;//return the result
}
float Gate::process(float input, float sideChain){//overload for sidechain input
  delayLine.inputSample(input);//delay line fed input signal still
  sideChain = sideChain * linearInputGain;//scale sidechain by linearInputGain
  updateGain(sideChain);//update final gain value with sideChain instead of input
  //the output is the delayed input scaled by this smoothed linearGain, then multiplied by makeup gain
  currentSample = delayLine.getDelayed(analysisTime + lookAhead) * 
                  linearGain.getCurrentValue() * linearMakeUpGain;
  return currentSample;//return the result
}
void Gate::updateGain(float input){
  //Find the linear intensity of scaled input signal
  float currentEstimate = signalEstimator.process(input);//store the estimate
  //debug.printOncePerBuffer(currentEstimate);
  if(currentEstimate < linearThreshold){//if tested signal is more quite than input
    //This next portion is particularily expensive as it must call both
    //amplitudeToDB and dBToAmplitude. Fortunately, this is only called
    //every time a higher currentEstimate is reached.
    if(currentEstimate < lowestAttackPhaseTarget){//should only occur if exceeds previous target
      lowestAttackPhaseTarget = currentEstimate;//record it as the highest
      attackFlag = true;//make sure compressor is in attack phase if it wasn't already
      attackPhaseInSamples = 0;//restart the attack phase
      //how many dB below the threshold was it? How many dB under?
      if(currentEstimate > 0.0f){//can't take amplitudeToDB of 0.0 (-inf)
        float differenceInDB = amplitudeToDB(currentEstimate) - threshold;
        //how much should the signal be scaled by?
        reductionTargetDB = (differenceInDB * (ratio)) - differenceInDB;
        reductionTargetDB -= rangeDB;//subtract rangeDB value 
        linearGain.setTarget(dBToAmplitude(reductionTargetDB));//ramp to target amplitue scalar
      }else{//if estimate was 0.0f, resulting in differenceInDB being -inf
        linearGain.setTarget(0.0f);//no need to do math, just set 
      }
    }
  }
  if(attackFlag){//if in attack phase
    attackPhaseInSamples++;//increment the attackPhase counter
    //if we have reached the end of the attack+hold phase
    if(attackPhaseInSamples >= attackTimeInSamples + holdTimeInSamples){
      lowestAttackPhaseTarget = 1000.0f;//reset the highest for next round
      attackPhaseInSamples = 0;//reset the attackPhase counter
      linearGain.setTarget(1.0f);//set target for release
      attackFlag = false;//change to release state
    }
  }
  linearGain.process();//update linear gain based on current target
}
void Gate::setThresholdDB(float thresholdDB){
  lowestAttackPhaseTarget = 1000.0;//previous value may no longer be relavant
  threshold = thresholdDB;//threshold is stored in decibels
  linearThreshold = dBToAmplitude(thresholdDB);//efficient to store this, since used often
}
void Gate::setRatio(float newRatio){//for every newRatio the input is above the output, reduce output to 1/newRatio over threshold
  ratio = std::fmax(newRatio, 1.0f);//assign the value, don't allow values below 1.0f
}
void Gate::setRangeDB(float newRangeDB){rangeDB = std::max(newRangeDB, 0.0f);}
void Gate::setInputGainDB(float newInputGainDB){linearInputGain = dBToAmplitude(newInputGainDB);}
void Gate::setInputGainLinear(float newLinearInputGain){linearInputGain  = newLinearInputGain;}
void Gate::setMakeUpGainDB(float newMakeUpGainDB){linearMakeUpGain = dBToAmplitude(newMakeUpGainDB);}
void Gate::setAttackTime(float newAttackTime){
  linearGain.setTimeDown(newAttackTime);
  attackPhaseInSamples = 0;//reset the attack phase
  attackTimeInSamples = msToSamples(newAttackTime);//adjust the attack phase counter
}
void Gate::setHoldTime(float newHoldTime){
  holdTimeInSamples = msToSamples(std::max(newHoldTime, 0.0f));
}
void Gate::setReleaseTime(float newReleaseTime){linearGain.setTimeUp(newReleaseTime);}
void Gate::setLookAheadTime(float newLookAheadTime){
  lookAhead = std::fmax(newLookAheadTime, 0.0f);
}
//lower times (8-32 samples) are more transient-sensitive
void Gate::setAnalysisTime(float newAnalysisTime){//defaults to 16 samples
  signalEstimator.setSamplePeriod(samplesToMS(newAnalysisTime));
}
float Gate::getSample(){return currentSample;}
//getLinearScalar() may be used to compress external signals
float Gate::getLinearScalar(){
  return linearGain.getCurrentValue();
}
float Gate::getThresholdDB(){return threshold;}
float Gate::getRatio(){return ratio;}
float Gate::getInputGainDB(){return amplitudeToDB(linearInputGain);}
float Gate::getMakeUpGainDB(){return dBToAmplitude(linearMakeUpGain);}
float Gate::getAttackTime(){return linearGain.getTimeUp();}
float Gate::getReleaseTime(){return linearGain.getTimeDown();}
float Gate::getLookAheadTime(){return lookAhead;}