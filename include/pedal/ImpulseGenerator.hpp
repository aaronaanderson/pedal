#ifndef ImpulseGenerator_hpp
#define ImpulseGenerator_hpp

#include "Settings.hpp"
#include "utilities.hpp"
#include <cmath>

namespace pedal{
class ImpulseGenerator{
  public:
  ImpulseGenerator();
  ImpulseGenerator(float initialFrequency);
  ~ImpulseGenerator();

  float generateSample();

  void setFrequency(float newFrequency);
  void setMaskChance(float newMaskChance);
  void setDeviation(float newDeviation);
  void setPhase(float newPhase);

  float getSample();
  float getFrequency();
  float getMaskChance();
  float getDeviation();
  
  float frequency, phase, period;
  float maskChance;
  float deviation, randomOffset;//deviation from periodicity
  float currentSample;
};
}//end pedal namespace
#endif