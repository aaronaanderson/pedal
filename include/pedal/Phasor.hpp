#ifndef Phasor_hpp
#define Phasor_hpp

#include "pdlConstants.hpp"
#include <cmath> 
#include "Settings.hpp"//so we can access sampleRate and bufferSize

namespace pedal{
class Phasor{//Pedal Trivial Sine Oscillator
  public:
  Phasor();//constructor, defined in the cpp
  Phasor(float frequency);//option to set frequency on construction
  ~Phasor();//deconstructor (may be needed to free memory)
  //DANGER, returns range 0 to TWOPI; do not output directly
  float generateSample();//calculate and return next sample
  //set
  void setFrequency(float newFrequency);
  void setPhase(float newPhase);
  //get
  float getFrequency();
  float getPhase();
  float getSample();
  //=======================================================
  private://best practice to leave inner workings private
  float frequency, phase, amplitude;//standard oscillator variables
  float currentSample;//current working sample
  double phaseIncrement;//extra precision necessary 
};
}//end pedal namespace
#endif 