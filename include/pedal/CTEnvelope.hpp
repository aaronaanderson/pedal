#ifndef CTEnvelope_hpp
#define CTEnvelope_hpp

#include <cmath>//for fmin and fmax
#include "Settings.hpp"//so we can access sampleRate and bufferSize
#include "utilities.hpp"// for clamp 

namespace pedal{
class CTEnvelope{//Constant-Time Envelope (linear piece-wise ADSR)
  public:
  CTEnvelope();//default constructor
  //best to have a standard ADSR as an overload constructor
  CTEnvelope(float initialAttack, float initialDecay, float initialSustain, float initialRelease);
  ~CTEnvelope();//deconstructor, used to clear memory if allocated
  
  enum class Mode {
    ADSR=0, 
    ASR, 
    AR
  };//3 envelope types
  void setup(float newAttack, float newDecay, float newSustain, float newRelease);
  float generateSample();//calculate and return next sample
  float* generateBlock();//calculate and return next block of samples
  
  float getAttackTime();
  float getDecayTime();
  float getSustainLevel();
  float getReleaseTime();
  float getSample();
  int getCurrentState();
  Mode getCurrentMode();
  bool getTrigger();
  bool isBusy();
  
  void setMode(Mode newMode);
  void setAttackTime(float newAttack);
  void setDecayTime(float newDecay);
  void setSustainLevel(float newSustain);
  void setReleaseTime(float newRelease);
  void setTrigger(bool newTrigger);
  //=======================================================
  private:
  enum State {OFF, ATTACK, DECAY, SUSTAIN, RELEASE};
  void calculateIncrement(State whichIncrement);//nead a 'state' variable

  int currentState;//which phase, off-a-d-s-r, is the envelope in
  Mode currentMode;//which type of envelope is it, adsr,asr, or ar
  bool trigger;//on or off
  float attack, decay, sustain, release;//internal values for these variables
  float attackIncrement, decayIncrement, releaseIncrement;//necessary to calculate next sample
  float currentSample;//current working sample
};
}//end pedal namespace
#endif