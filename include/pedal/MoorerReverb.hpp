#ifndef MoorerReverb_hpp
#define MoorerReverb_hpp

#include "pedal/LowPassCombFilter.hpp"
#include "pedal/AllPass.hpp"
#include "pedal/CircularBuffer.hpp"
#include "pedal/utilities.hpp"
#define NUM_COMBS 6
#define NUM_TAPS 18

class MoorerReverb{
  public:
  MoorerReverb();

  float process(float input);
  float getSample();
  float setReverbTime(float newReverbTime);//not safe for audio thread
  float setDryWet(float newDryWet);

  private:
  float currentSample;
  float time;//time to -60dBFS
  float dryWetMix;
  AllPass allPass;
  CircularBuffer delayLine;//buffer for multi-tap delay (early reflections)
  const float delayTimes[NUM_TAPS] = {4.3f, 21.5f, 22.5f, 26.8f,
                                27.0f, 29.8f, 45.8f, 48.5f, 
                                57.2f, 58.7f, 59.55f, 61.2f, 
                                70.7, 70.8f, 72.6f, 74.1f, 
                                75.3f, 79.7f};
  const float delayGains[NUM_TAPS] = {0.841f, 0.504f, 0.491f, 0.379f, 
                                0.380f, 0.346f, 0.289f, 0.272f, 
                                0.192f, 0.192f, 0.217f, 0.181f, 
                                0.180f, 0.181f, 0.176f, 0.142f, 
                                0.167f, 0.134f};
  LowPassCombFilter filterBank[NUM_COMBS];//an array of lowpass comb filters
  const float combDelayTimess[NUM_COMBS] = {50.0f, 56.0f, 61.0f,
                                          68.0f, 72.0f, 78.0f};
  const float combFilterFrequencies[NUM_COMBS] = {1942.0f, 1363.0f, 
                                                 1312.0f, 1574.0f, 
                                                 981.0f, 1036.0f};
  //TODO Make this a AoS instead of SoA

    
};

/*
comb filter settings and delay times/gains were sourced from 
https://christianfloisand.wordpress.com/2012/10/18/algorithmic-reverbs-the-moorer-design/
*/
#endif