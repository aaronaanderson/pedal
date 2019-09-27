#ifndef CTEnvelope_hpp
#define CTEnvelope_hpp

#include "pdlSettings.hpp"//so we can access sampleRate and bufferSize


class CTEnvelope{
  public:
  CTEnvelope();
  ~CTEnvelope();

  float generateSample();
  float* generateBlock();

  float getAttack();
  float getDecay();
  float getSustain();
  float getRelease();

  void setup(float newAttack, float newDecay, float newSustain, float newRelease);
  void setMode(mode newMode);
  void setAttack(float newAttack);
  void setDecay(float newDecay);
  void setRelease(float newRelease);
  void setSustain(float newSustain);

  private:
  
  enum state {OFF=0, ATTACK, DECAY, SUSTAIN, RELEASE};
  enum mode {ADSR=0, AD, AR, ASR};
  void setIncrement(state whichIncrement);//nead a 'state' variable
  float attack, decay, sustain, release;
  float attackIncrement, decayIncrement, releaseIncrement;
  float currentSample;
  float* currentBlock;
};

#endif