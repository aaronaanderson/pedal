#include "CTEnvelope.hpp"

CTEnvelope::CTEnvelope(){
    setup(100, 40, 0.7, 400);
}

CTEnvelope::CTEnvelope(float initialAttack, float initialDecay, float initialSustain, float initialRelease){
    setup(initialAttack, initialDecay, initialSustain, initialRelease);
}

CTEnvelope::~CTEnvelope(){
    if(currentBlock != nullptr){//if block exists
        delete currentBlock;//free the memory
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
  attack = newAttack;
  decay = newDecay;
  sustain = newSustain;
  release = newRelease;
  calculateIncrement(ATTACK);
  calculateIncrement(DECAY);
  calculateIncrement(RELEASE);
  currentSample = 0.0;
  currentState = states::OFF;
}

void CTEnvelope::calculateIncrement(states whichIncrement){
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