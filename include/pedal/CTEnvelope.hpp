#ifndef CTEnvelope_hpp
#define CTEnvelope_hpp

#include "pdlSettings.hpp"//so we can access sampleRate and bufferSize

class CTEnvelope{
  public:
  CTEnvelope();
  CTEnvelope(float initialAttack, float initialDecay, float initialSustain, float initialRelease);
  ~CTEnvelope();
  
  
  float generateSample();
  float* generateBlock();

  float getAttack();
  float getDecay();
  float getSustain();
  float getRelease();

  enum mode {ADSR=0, AD, AR, ASR};
  void setup(float newAttack, float newDecay, float newSustain, float newRelease);
  void setMode(mode newMode);
  void setAttack(float newAttack);
  void setDecay(float newDecay);
  void setRelease(float newRelease);
  void setSustain(float newSustain);
 
  private:
  float calculateNextSample();
  enum states {OFF=0, ATTACK, DECAY, SUSTAIN, RELEASE};
  void calculateIncrement(states whichIncrement);//nead a 'state' variable
  int currentState;
  float attack, decay, sustain, release;
  float attackIncrement, decayIncrement, releaseIncrement;
  float currentSample;
  float* currentBlock;
};

#endif