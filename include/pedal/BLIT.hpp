#ifndef BLIT_hpp
#define BLIT_hpp

#include "pedal/pdlSettings.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

//Band Limited Impulse Train
class BLIT{
  public:
  BLIT();
  float generateSample();

  void setNumberOfHarmonics(float newNumberOfHarmonics);
  void setFrequency(float newfrequency);
  float getNumberOfHarmonics();
  float getFrequency();
  float getPhase();

  private:
  float currentSample;
  float numberOfHarmonics;
  float frequency;
  float phase;
  int periodInSamples;
  double phaseIncrement;
};
#endif