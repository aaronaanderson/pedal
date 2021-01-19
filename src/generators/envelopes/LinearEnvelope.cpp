#include "pedal/LinearEnvelope.hpp"

using namespace pedal;

//=========================================================
LinearEnvelope::LinearEnvelope(){
 setup(100, 40, 0.7, 400);//simple default values
}
LinearEnvelope::LinearEnvelope(float initialAttack, float initialDecay, float initialSustain, float initialRelease){
  setup(initialAttack, initialDecay, initialSustain, initialRelease);
}
LinearEnvelope::~LinearEnvelope(){}//when object is deleted
//=========================================================
void LinearEnvelope::setup(float newAttack, float newDecay, float newSustain, float newRelease){
  setSustainLevel(newSustain);//clamp sustain 0.0 to 1.0
  setAttackTime(newAttack);
  setDecayTime(newDecay);
  setReleaseTime(newRelease);
  currentSample = 0.0;
  currentState = State::OFF;
  currentMode = Mode::ADSR;
}
float LinearEnvelope::generateSample(){//generate a single sample
  switch(currentMode){//which mode (ADSR, ASR, or AR)
    case Mode::ADSR://Attack, Decay, Sustain, Release (default)
      switch(currentState){//envelope behaves differently based on which state it is in
        case OFF:
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
    case Mode::ASR://Attack, Sustain, Release
      switch(currentState){
        case OFF:
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
    case Mode::AR://Attack, Release (useful for percusive envelopes)
      switch(currentState){
        case OFF:
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
void LinearEnvelope::calculateIncrement(State whichIncrement){
  //these are the values that are added or subtracted to currentSample
  //depending on the current state of the envelope. These values will be 
  //dependent on the sampling rate, the duration of the state, and where it 
  //going next.
  switch(whichIncrement){
    case ATTACK:
      attackIncrement = 1.0f/(Settings::sampleRate * (attack * 0.001f));
    break;
    case DECAY:
      decayIncrement = (1.0f - sustain)/(Settings::sampleRate * (decay * 0.001f));
    break;
    //SUSTAIN has no increment; it is a measure of level, not time
    case RELEASE:
      if(currentMode == Mode::AR){
        releaseIncrement = 1.0f/(Settings::sampleRate * (release * 0.001f));
      }else{
        releaseIncrement = sustain/(Settings::sampleRate * (release * 0.001f));
      }
    break;
  }
} 

//=========================================================
float LinearEnvelope::getAttackTime(){return attack;}
float LinearEnvelope::getDecayTime(){return decay;}
float LinearEnvelope::getSustainLevel(){return sustain;}
float LinearEnvelope::getReleaseTime(){return release;}
float LinearEnvelope::getSample(){return currentSample;}
int LinearEnvelope::getCurrentState(){return currentState;}
LinearEnvelope::Mode LinearEnvelope::getCurrentMode(){return currentMode;}
bool LinearEnvelope::getTrigger(){return trigger;}
bool LinearEnvelope::isBusy(){
  if(currentState != OFF){
    return true;
  }
  return false;
}
void LinearEnvelope::setMode(Mode newMode){currentMode = newMode;}
void LinearEnvelope::setAttackTime(float newAttack){//any positive value
  attack = newAttack;
  calculateIncrement(ATTACK);//changing value requires recalculating increment
}
void LinearEnvelope::setDecayTime(float newDecay){//any positive value
  decay = newDecay;
  calculateIncrement(DECAY);//changing value requires recalculating increment
}
void LinearEnvelope::setSustainLevel(float newSustain){//amplitude from 0.0 to 1.0
  sustain = newSustain;
  if(currentMode == Mode::ADSR || currentMode == Mode::ASR){//protect a AR user from themselves
    calculateIncrement(DECAY);//New sustain value will change this value
    calculateIncrement(RELEASE);//New sustain value will changes this value
  }
}
void LinearEnvelope::setReleaseTime(float newRelease){//any positive value
  release = newRelease;
  calculateIncrement(RELEASE);//changing vale requires recalculating increment
}
void LinearEnvelope::setTrigger(bool newTrigger){
  if(newTrigger == true && trigger == false){currentState = ATTACK;}
  if(newTrigger == false && trigger == true){currentState = RELEASE;}
  if(currentMode == Mode::AR){
    trigger = false;
  }else{
    trigger = newTrigger;
  }
}