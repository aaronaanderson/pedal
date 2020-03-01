#ifndef MoorerReverb_hpp
#define MoorerReverb_hpp

#include <cmath>
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
  float getG1(){return g1;}
  private:
  float currentSample = 0.0f;//begin with a 0 sample
  float filterOutput = 0.0f;
  CircularBuffer delayLine;//needed for comb filter
  float delayTime;//in ms
  float g1, g2;//naming convention same as paper
};

//store these together, since used together
struct Tap{
  float time;
  float amplitude;
};
/*
This reverb is an assemblage of a multi-tap delay, 
6 comb filters with internal LPFs (designed above), 
and an allpass filter. It is very computationally 
cheap as far as reverbs go. 
*/
class MoorerReverb{
  public:
  MoorerReverb();

  float process(float input);
  float getSample();
  float getG();
  float getReverbTime();
  float getDryWetMix();
  void setReverbTime(float newReverbTime);
  void setDryWetMix(float newDryWet);
  void setG(float newG);

  private:
  float currentSample;//store the working sample
  float g;//same notation from paper. this can be considered global feedback
  float time;//time to -60dBFS (only roughly estimated)
  float dryWetMix;//useful if using this in a synth, not in original design
  AllPass allPass;//Moorer argues that only one allpass is useful
  CircularBuffer delayLine;//buffer for multi-tap delay (early reflections)
  //18 delay times and amplitudes for early reflections, taken from paper {time(ms), amplitude}
  const Tap taps[NUM_TAPS] = {{4.3f, 0.841}, {21.5f, 0.504f},
                              {22.5f, 0.491f}, {26.8f, 0.379f},
                              {27.0f, 0.380f}, {29.8f, 0.346f},
                              {45.8f, 0.289f}, {48.5f, 0.272f},
                              {57.2f, 0.192f}, {58.7f, 0.192f},
                              {59.55f, 0.217f}, {61.2f, 0.181f},
                              {70.7f, 0.180f}, {70.8f, 0.181f},
                              {72.6f, 0.176f}, {74.1f, 0.142f},
                              {75.3f, 0.167f}, {79.7f, 0.134f}};
  MoorerReverberationUnit filterBank[NUM_COMBS];//an array of lowpass comb filters  
  //we'll use the paper's suggestion and use their g1 values at sample rates
  //25k and 50k to interpolate/extrapolate gain values for our
  //sample rate. This is taken from the data in Table 2
  const float gainArray[NUM_COMBS] = {//This is how the cutoff frequency of internal lpf in combs is set, by g1
    linearInterpolation(pdlSettings::sampleRate, 25000.0f, 0.24f, 50000.0f, 0.46f), 
    linearInterpolation(pdlSettings::sampleRate, 25000.0f, 0.26f, 50000.0f, 0.48f),
    linearInterpolation(pdlSettings::sampleRate, 25000.0f, 0.28f, 50000.0f, 0.50f),
    linearInterpolation(pdlSettings::sampleRate, 25000.0f, 0.29f, 50000.0f, 0.52f),
    linearInterpolation(pdlSettings::sampleRate, 25000.0f, 0.30f, 50000.0f, 0.53f),
    linearInterpolation(pdlSettings::sampleRate, 25000.0f, 0.32f, 50000.0f, 0.55f)};
  //delay times, taken from Table 2
  const float combDelayTimess[NUM_COMBS] = {50.0f, 56.0f, 61.0f,
                                            68.0f, 72.0f, 78.0f};
  const float reverbLagTime = 78.0f + 6.0f;//longest comb time + allpass time 
  CircularBuffer lagBuffer;
};
#endif