#ifndef TPhasor_hpp
#define TPhasor_hpp

#include "pdlConstants.hpp"
#include <cmath> 
#include "pdlSettings.hpp"//so we can access sampleRate and bufferSize

namespace pedal{
class TPhasor{//Pedal Trivial Sine Oscillator
  public:
  TPhasor();//constructor, defined in the cpp
  TPhasor(float frequency);//option to set frequency on construction
  ~TPhasor();//deconstructor (may be needed to free memory)
  float generateSample();//calculate and return next sample
  float* generateBlock();//calculate and return next block of samples
    //set
    void setFrequency(float newFrequency);
    void setPhase(float newPhase);
    void setAmplitude(float newAmplitude);
    //get
    float getFrequency();
    float getPhase();
    float getAmplitude();
    float getSample();
    //=======================================================
    private://best practice to leave inner workings private
    float frequency, phase, amplitude;//standard oscillator variables
    float currentSample;//current working sample
    double phaseIncrement;//extra precision necessary 
};
}//end pedal namespace
#endif 