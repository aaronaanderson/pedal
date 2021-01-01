/*
// This is implementation was derived from the following paper: 
// Vesa Välimäki, Oscillator and Filter Algorithms for Virtual Analog Synthesis - 2006
// The paper discusses both a Moog Ladder Filter implementation, and the DPW synthesis
// implementation. This document focuses on the Ladder filter
//
// The Moog Ladder filter is commonly considered to be the first 'musical' filter, invented
// in its analog form in 1965 by Robert Moog: Moog, R. A. 1965. “Voltage-Controlled Electronic Music Modules.” Journal of the Audio Engineering Society 13(3):200–206.
// By musical, I simply mean that this was invented with musical purposes in mind: Indpendently 
// controllable cutoff frequency and resonance, and a 'musically pleasing' frequency response rather
// than an exceptionally accuracte and predictable response. Afterall, filters existed well 
// before they were used for musical purposes
//
// Stiltson and Smith are creditied for creating a digital form of this analog filter in 1996: 
// Stilson, T., and J. O. Smith. 1996b. “Analyzing the Moog VCF with Considerations for Digital Implementation.” 
//
// Huovilainen improved the model in 2004 by adding nonlinearity into the feedback loop of the IIR lowpass filters.
// Essentially, this is a wave shaper put on each inner lowpass filter for harmonic distortion. Välimäki, the author 
// of our implementation here, kept this harmonic distortion, but only kept one in the global loop (as apposed to 4
// in all of the inner lowpass loops) for computational performance reasons.
//
// A 'Moog Ladder Filter' usually refers to a 24dB per octave resonant lowpass filter. However, analog designs (by 
// Oberheim Electronics) of this filter were augmented in the 1980s to offer variable filter modes and
// falloff rates. While expensive to pull off in an analog implementation (and therefore somewhat 
// rare), we simply need to keep a table of scalar values for our digital implementation
//
// Aaron Anderson, Oct 2020
*/
#ifndef MoogLadderFilter_hpp
#define MoogLadderFilter_hpp

#include <vector>
#include <iostream>
#define _USE_MATH_DEFINES
#include <cmath>

#include "pedal/pdlSettings.hpp"
#include "pedal/utilities.hpp"

#define NUM_SUB_FILTERS 4
#define NUM_MODE_SCALARS 5


static const float modeTable[6][5] = {{0.0f, 0.0f, 1.0f, 0.0f, 0.0f},
                                      {0.0f, 0.0f, 0.0f, 0.0f, 1.0f},
                                      {0.0f, 2.0f, -2.0f, 0.0f, 0.0f},
                                      {0.0f, 0.0f, 4.0f, -8.0f, 4.0f},
                                      {1.0f, -2.0f, 1.0f, 0.0f, 0.0f}, 
                                      {1.0f, -4.0f, 6.0f, -4.0f, 1.0f}};

namespace pedal{
class MoogLadderFilter{
    public:
    MoogLadderFilter();
    ~MoogLadderFilter(){}
    //the one pole filter section is special to this filter
    class CompromiseOnePoleFilter;//defined below
    enum class MODE{
        TWOPOLE_LOWPASS, 
        FOURPOLE_LOWPASS,
        TWOPOLE_BANDPASS,
        FOURPOLE_BANDPASS,
        TWOPOLE_HIGHPASS,
        FOURPOLE_HIGHPASS
    };
    inline float processSample(float inputSample);
    void setMode(MoogLadderFilter::MODE newMode);
    void setFrequency(float newFrequency);
    void setGain(float newGain);
    void setResonance(float newResonance);

    private:
    float previousOutputSample = 0.0f;
    float inputCompensation = 0.5f;
    float resonance;
    float normalizedAngularFrequency;
    std::vector<CompromiseOnePoleFilter> filterArray;
    float compensateFrequency(float intendedFrequency);
    float compensateResonance(float intendedResonance);
    void setModeScalarsFromTable();
    MODE currentMode;
    float modeScalars[5];//0-4 instead of A-E as in paper
};

class MoogLadderFilter::CompromiseOnePoleFilter{
    public:
    inline float process(float inputSample){
        float outputSample;
        //this filter has two sections
        //the zero -0.3 (to decorelate frequency and resonance)
        outputSample = (previousInput * small) + (inputSample * big);
        //and the IIR lowpass section
        outputSample -= previousOutput;
        outputSample *= g;
        outputSample += previousOutput;

        //update the previous samples for next round
        previousInput = inputSample;
        previousOutput = outputSample;
        return outputSample;
    }
    void setG(float newG){g = newG;}
    private:
    float previousInput = 0.0f;
    float previousOutput = 0.0f;
    float g;
    //TODO find better names fore these
    constexpr static const float small = 0.3f/1.3f;
    constexpr static const float big = 1.0f/1.3f;
};

float MoogLadderFilter::processSample(float inputSample){
  float outputSample = 0.0f;
  outputSample = (( (-inputSample * inputCompensation) + previousOutputSample) * resonance * -4.0f) + inputSample;
  outputSample = std::tanh(outputSample);//we'll seee....
  float outputAccumulator = 0.0f;
  for (int i = 0; i < NUM_SUB_FILTERS; i++) {
    outputAccumulator += modeScalars[i] * outputSample;
    outputSample = filterArray[i].process(outputSample);
  }
  previousOutputSample = outputSample;//set for next iteration
  outputSample *= modeScalars[4];
  outputSample += outputAccumulator;
  return outputSample;
}
}//end pedal namespace
#endif
