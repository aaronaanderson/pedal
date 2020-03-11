#include "pedal/ImpulseGenerator.hpp"
#include "iostream"
//Constructors and Deconstructors=========
ImpulseGenerator::ImpulseGenerator(){
  setFrequency(1.0f);//one impulse per second
  setPhase(0.0f);//initialize phase to 0
  setDeviation(0.0f);//ensure periodicity
  setMaskChance(0.0f);//no missing impulses
  randomOffset = rangedRandom(-period*0.5, period*0.5) * deviation;
}
ImpulseGenerator::ImpulseGenerator(float initialFrequency){
  setFrequency(initialFrequency);
  setPhase(0.0f);//initialize phase to 0
  setDeviation(0.0f);//ensure periodicity
  setMaskChance(0.0f);//no missing impulses
  randomOffset = rangedRandom(-period*0.5, period*0.5) * deviation;
}
ImpulseGenerator::~ImpulseGenerator(){
    delete[] currentBlock;
}
//core functionality======================
float ImpulseGenerator::generateSample(){
  if(phase >= period+randomOffset){
    float test = rangedRandom(0.0f, 1.0f);
    if(test > maskChance){
      currentSample = 1.0f;
      float halfPeriod = period*0.5f;
      randomOffset = rangedRandom(-halfPeriod, halfPeriod) * deviation;
    }
    phase -= period;
  }else{
    currentSample = 0.0f;
    phase += 1.0f;//increase phase by 1 sample 
  }
  return currentSample;
}
float* ImpulseGenerator::generateBlock(){
  if(currentBlock == nullptr){
    currentBlock = new float[pdlSettings::bufferSize];
  }
  for(int i = 0; i < pdlSettings::bufferSize; i++){
    currentBlock[i] = generateSample();
  }
  return currentBlock;
}
//Getters and Setters======================
void ImpulseGenerator::setFrequency(float newFrequency){
  frequency = fabs(newFrequency);//no need for negative frequencies for this
  period = pdlSettings::sampleRate/frequency;//period in samples
}
void ImpulseGenerator::setPhase(float newPhase){
    phase = newPhase*period;//conver from 0 - 1 to 0 - period
}
void ImpulseGenerator::setMaskChance(float newMaskChance){maskChance = newMaskChance;}
void ImpulseGenerator::setDeviation(float newDeviation){deviation = newDeviation;}

float ImpulseGenerator::getSample(){return currentSample;}
float* ImpulseGenerator::getBlock(){return currentBlock;}
float ImpulseGenerator::getFrequency(){return frequency;}
float ImpulseGenerator::getMaskChance(){return maskChance;}
float ImpulseGenerator::getDeviation(){return deviation;}
