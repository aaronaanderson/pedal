#ifndef BLIT_hpp
#define BLIT_hpp

#define _USE_MATH_DEFINES
#include <cmath>
#include <limits>

#include "pedal/pdlSettings.hpp"
#include "pedal/DebugTool.hpp"
//Band Limited Impulse Train
class BLIT{
  public:
  BLIT();
  float generateSample();

  void setNumberOfHarmonics(float newNumberOfHarmonics);
  void setFrequency(float newfrequency);
  void setPhase(float newPhase);//0 to PI
  void setSyncHarmonicsToFrequency(bool newSynchHarmonicsToFrequency);
  float getNumberOfHarmonics();
  float getFrequency();
  float getPhase();
  bool getSyncHarmonicsToFrequency();
  private:
  float currentSample;
  float numberOfHarmonics;
  bool syncHarmonicsWithFrequency;
  float frequency;//Emission rate of Band Limited Impulses
  float phase;//location within the band limited impulse
  double phaseIncrement;
  DebugTool dT;
};
#endif