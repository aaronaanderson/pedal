#ifndef MoorerReverb_hpp
#define MoorerReverb_hpp

#include "pedal/AllPass.hpp"
#include "pedal/CircularBuffer.hpp"
#include "pedal/utilities.hpp"
#define NUM_COMBS 6
#define NUM_TAPS 18
/*
James Moorer "About this Reverberation Business" - 1979
This paper discusses modern (to 1979) techniques in digital 
reverb. Naming and conventions are kept identical and clear
to the paper, when possible. This model is a significant
improvement to Schroeder's model, but, while it can produce
useful results, it is include mostly for academic purposes.
*/
//First a reverberation unit is needed
//Moorer augmented Schroeder's design by adding 
//a one-pole lowpass filter in the feedback loop
//of a comb filter. Below is the minimal 
//design, as discussed in the paper. I will only use this 
//here, in MoorerReverb, so I will make it specificallly
//for the those needs.
class MoorerReverberationUnit{//Fig. 5 in paper
  public:
  float process(float input){
    //currentSample is the delayLine output at this point
    filterOutput = currentSample + (filterOutput*g1);//LPF if g1 positive
    //feed the delay line  the input + a scaled output of the LPF
    delayLine.inputSample((filterOutput*g2) + input);
    currentSample = delayLine.getDelayed(delayTime);
    return currentSample;
  }
  void setG1(float newG1){g1 = newG1;}//LPF feedback gain
  void setG2(float newG2){g2 = newG2;}//comb feedback gain
  void setCombDelayTime(float newDelayTime){delayTime = newDelayTime;}
  private:
  float currentSample = 0.0f;//begin with a 0 sample
  float filterOutput = 0.0f;
  CircularBuffer delayLine;//needed for comb filter
  float delayTime;//in ms
  float g1, g2;//naming convention same as paper
};

/*
This reverb is an assemblage of a multi-tap delay, 
6 comb filters with internal LPFs (designed above), 
and an allpass filter.
*/
struct Tap{
  float time;
  float amplitude;
}

class MoorerReverb{
  public:
  MoorerReverb();

  float process(float input);
  float getSample();
  float setReverbTime(float newReverbTime);
  void setDryWetMix(float newDryWet);
  void setG(float newG);//TODO

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
  MoorerReverberationUnit filterBank[NUM_COMBS];//an array of lowpass comb filters
  const float combDelayTimess[NUM_COMBS] = {50.0f, 56.0f, 61.0f,
                                          68.0f, 72.0f, 78.0f};     
  //TODO Make this a AoS instead of SoA 
  const Tap taps[NUM_TAPS] = {{4.3f, 0.841}, {21.5f, 0.504f},
                              {22.5f, 0.491f}, {26.8f, 0.379f},
                              {27.0f, 0.380f}, {29.8f, 0.346f},
                              {45.8f, 0.289f}, {48.5f, 0.272f},
                              {57.2f, 0.192f}, {58.7f, 0.192f},
                              {59.55f, 0.217f}, {61.2f, 0.181f},
                              {70.7f, 0.180f}, {70.8f, 0.181f},
                              {72.6f, 0.176f}, {74.1f, 0.142f},
                              {75.3f, 0.167f}, {79.7f, 0.134f}};
};


#endif