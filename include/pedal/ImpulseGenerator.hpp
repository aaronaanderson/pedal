#ifndef ImpulseGenerator_hpp
#define ImpulseGenerator_hpp

#include "pdlSettings.hpp"
#include "utilities.hpp"
#include "math.h"

class ImpulseGenerator{
  public:
  ImpulseGenerator();
  ImpulseGenerator(float initialFrequency);
  ~ImpulseGenerator();

  float generateSample();
  float* generateBlock();

  void setFrequency(float newFrequency);
  void setMaskChance(float newMaskChance);
  void setDeviation(float newDeviation);
  void setPhase(float newPhase);

  float getSample();
  float* getBlock();
  float getFrequency();
  float getMaskChance();
  float getDeviation();
  
  private:
  float frequency, phase, period;
  float maskChance;
  float deviation, randomOffset;//deviation from periodicity
  float currentSample;
  float* currentBlock = nullptr;
};
#endif