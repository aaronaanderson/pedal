#include "pedal/CTEnvelope.hpp"

//constructors and deconstructors
//=========================================================
CTEnvelope::CTEnvelope(){
 setup(100, 40, 0.7, 400);//simple default values
}

CTEnvelope::CTEnvelope(float initialAttack, float initialDecay, float initialSustain, float initialRelease){
  setup(initialAttack, initialDecay, initialSustain, initialRelease);
}

CTEnvelope::~CTEnvelope(){//deconstructor
  if(currentBlock != nullptr){//if block exists
    delete[] currentBlock;//free the memory
  }
}

//primary mechanics of class
//=========================================================
void CTEnvelope::setup(float newAttack, float newDecay, float newSustain, float newRelease){
  setSustain(newSustain);//clamp sustain 0.0 to 1.0
  setAttack(newAttack);
  setDecay(newDecay);
  setRelease(newRelease);
  currentSample = 0.0;
  currentState = states::OFF;
  currentMode = modes::ADSR;
}

float CTEnvelope::generateSample(){//generate a single sample
  switch(currentMode){//which mode (ADSR, ASR, or AR)
    case modes::ADSR://Attack, Decay, Sustain, Release (default)
      switch(currentState){//envelope behaves differently based on which state it is in
        case OFF:
          if(trigger){
              currentState = ATTACK;
          }
        break;
        case ATTACK://initial portion of a 'note'
          currentSample += attackIncrement;//increase value one sample at a time
          if(currentSample >= 1.0){//once the peak has been reached
            currentSample = 1.0;//don't let it spill over
            currentState = DECAY;//switch to the next state
          }
        break;
        case DECAY://small attenuation after attack and before sustain
          currentSample -= decayIncrement;//decrease value one sample at a time
          if(currentSample <= sustain){//once the sustain leve lis reached
            currentSample = sustain;//don't let it over shoot
            currentState = SUSTAIN;//switch to the next state
          }
        break;
        case SUSTAIN://the sustained, or 'held' portion of a 'note'
          if(!trigger){//if note is not being held
            currentState = RELEASE;
          }
        break;
        case RELEASE://the tail-end of a 'note'
          currentSample -= releaseIncrement;//decrease value sample by sample
          if(currentSample <= 0.0){//if less than 0
            currentSample = 0.0;
            currentState = OFF;//the enevelope has finished, mark as 'off'
          }
        break;
      }
    break;
    case modes::ASR://Attack, Sustain, Release
      switch(currentState){
        case OFF:
          if(trigger){
              currentSample = SUSTAIN;
          }
        break;
        case ATTACK://initial portion of a 'note'
        currentSample += attackIncrement;//increase value one sample at a time
        if(currentSample >= sustain){//stop at the sustain level (no decay portion in this mode)
          currentSample = sustain;//don't let it over shoot
          currentState = SUSTAIN;//switch to next state
        }
        break;
        case SUSTAIN://the sustained, or 'held' portion of a 'note'
          if(!trigger){
              currentState = RELEASE;
          }
        break;
        case RELEASE://the tail-end of a 'note'
        currentSample -= releaseIncrement;//decrease value sample by sample
        if(currentSample <= 0.0){//once we've arrived
          currentSample = 0.0;//the envelope will only report 0.0
          currentState = OFF;//switch to the off state
        }
        break;
      }
    break;
    case modes::AR://Attack, Release (useful for percusive envelopes)
      switch(currentState){
        case OFF:
          if(trigger){
              currentState = ATTACK;
          }
        break;
        case ATTACK://initial portion of a 'note'
        currentSample += attackIncrement;//increase value one sample at a time
          if(currentSample >= 1.0){//stop at peak
            currentSample = 1.0;//don't let it overshoot
            currentState = RELEASE;//move to next state
          }
        break;
        case RELEASE://the tail-end of a 'note'
        currentSample -= releaseIncrement;//decrease value sample by sample
        if(currentSample <= 0.0){//once we've arrived
          currentSample = 0.0;//only report 0.0 until on again
          currentState = OFF;//update the state to off
          trigger = false;
        }
        break;
      }
    break;
  }//end of mode switch statement
  return currentSample;
}

float* CTEnvelope::generateBlock(){
  //check if block has been allocated in memory yet
  if(currentBlock == nullptr){
    currentBlock = new float[pdlSettings::bufferSize];
  }
  //fill the block with samples
  for(int i = 0; i < pdlSettings::bufferSize; i++){
    currentBlock[i] = generateSample();
  }
  return currentBlock;//return (a pointer to) the block
}

void CTEnvelope::calculateIncrement(states whichIncrement){
  //these are the values that are added or subtracted to currentSample
  //depending on the current state of the envelope. These values will be 
  //dependent on the sampling rate, the duration of the state, and where it 
  //going next.
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

//Getters and setters
//=========================================================
float CTEnvelope::getAttack(){return attack;}
float CTEnvelope::getDecay(){return decay;}
float CTEnvelope::getSustain(){return sustain;}
float CTEnvelope::getRelease(){return release;}
float CTEnvelope::getSample(){return currentSample;}
float* CTEnvelope::getBlock(){return currentBlock;}
int CTEnvelope::getCurrentState(){return currentState;}
CTEnvelope::modes CTEnvelope::getCurrentMode(){return currentMode;}
bool CTEnvelope::getTrigger(){return trigger;}
bool CTEnvelope::isBusy(){
  if(currentState == OFF){
    return false;
  }else{
    return true;
  }
  return false;//for windows compiler, should never happen
}

void CTEnvelope::setMode(modes newMode){currentMode = newMode;}
void CTEnvelope::setAttack(float newAttack){//any positive value
  attack = newAttack;
  calculateIncrement(ATTACK);//changing value requires recalculating increment
}
void CTEnvelope::setDecay(float newDecay){//any positive value
  decay = newDecay;
  calculateIncrement(DECAY);//changing value requires recalculating increment
}
void CTEnvelope::setSustain(float newSustain){//amplitude from 0.0 to 1.0
  sustain = newSustain;
  if(currentMode == modes::ADSR || currentMode == modes::ASR){//protect a AR user from themselves
    calculateIncrement(DECAY);//New sustain value will change this value
    calculateIncrement(RELEASE);//New sustain value will changes this value
  }
}
void CTEnvelope::setRelease(float newRelease){//any positive value
  release = newRelease;
  calculateIncrement(RELEASE);//changing vale requires recalculating increment
}
void CTEnvelope::setTrigger(bool newTrigger){
  if(newTrigger == true && trigger == false){currentState = ATTACK;}
  if(newTrigger == false && trigger == true){currentState = RELEASE;}
  trigger = newTrigger;
}