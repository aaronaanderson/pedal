//Aaron Anderson, 2020
#ifndef VOSIM_hpp
#define VOSIM_hpp
//VOSIM: 'VOice SIMulation'
//Created using resources 'VOSIM - A New Sound Synthesis Sysmtem' by Kaegi and Tempelaars
//and 'Signals, Systems, and Sound Syntesis' by Neukom pg 324-5
//as source material

#include "pdlConstants.hpp"
#include <cmath>
#include "pedal/pdlSettings.hpp"
#include <iostream>

namespace pedal{
class VOSIM{
  public:
  VOSIM();

  float generateSample();
  void setFrequency(float newFrequency);
  void setFormantFrequency(float newFormantFrequency);
  void setDecayFactor(float newDecayFactor);
  void setOscillationsPerPeriod(int newOscillationsPerPeriod);
  float getSample();

  private:
  void updateFrequency();
  void updateFormantFrequency();
  float nextFrequency;
  float nextFormantFrequency;
  float currentSample;
  float frequency;//fundamental
  float formantFrequency;
  float decayFactor;//how much each oscillation decays
  float delayBeforeNextWavefront;
  float localAmplitude;
  double oscillationPhaseIncrement;
  float oscillationPhase;//phase of individual sin curves
  double periodPhaseIncrement;
  float periodPhase;//phase in entire period
  int oscillationsPerPeriod;
  int nextOscillationsPerPeriod;//store until safe to 
  int currentOscillation;
  int maxOscillationsPerPeriod;//how many sinusuoidal oscillations can fit in the period
};
}//end pedal namespace
#endif