#include "CTEnvelope.hpp"

CTEnvelope::CTEnvelope(){
    setup(100, 40, 0.7, 400);
}
CTEnvelope::~CTEnvelope(){
    if(currentBlock != nullptr){
        delete currentBlock;
    }
}

float CTEnvelope::calculateNextSample(){

    return currentSample;
}

float CTEnvelope::generateSample(){
  currentSample = calculateNextSample();//calculate a single sample
  return currentSample;//return a single sample
}

float* CTEnvelope::generateBlock(){
  //check if block has been allocated in memory yet
  if(currentBlock == nullptr){
      currentBlock = new float[pdlSettings::bufferSize];
  }
  //fill the block with samples
  for(int i = 0; i < pdlSettings::bufferSize; i++){
      currentBlock[i] = calculateNextSample();
  }
  return currentBlock;//return the block
}

void CTEnvelope::setup(float newAttack, float newDecay, float newSustain, float newRelease)){
  calculateIncrement(ATTACK);
  calculateIncrement(DECAY);
  sustain = newSustain;
  calculateIncrement(RELEASE);
}

void CTEnvelope::calculateIncrement(state whichIncrement){
  switch(whichIncrement){
    case ATTACK:
      attackIncrement = 1/(pdlSettings::sampleRate * (attack * 0.001));
    break;
    case DECAY:
      decayIncrement = (1.0 - sustain)/(pdlSettings::sampleRate * (decay * 0.001));
    break;
    //SUSTAIN has no increment; it is a measure of level, not time
    case RELEASE:
      releaseIncrement = sustain/(pdlSettings::sampleRate * (release * 0.001));
    break;
  }
} 