
#ifndef TrivialTriangle_hpp
#define TrivialTriangle_hpp

#include "pdlConstants.hpp"
#include <cmath> //so we can use sin()
#include "Settings.hpp"//so we can access sampleRate and bufferSize

namespace pedal{
class TrivialTriangle{//Pedal Trivial Sine Oscillator
  public:
  TrivialTriangle();//constructor, defined in the cpp
  TrivialTriangle(float frequency);//option to set frequency on construction
  ~TrivialTriangle();//deconstructor (may be needed to free memory)
  float generateSample();//calculate and return next sample
  float generateSample(float fromPhase);//phase driven overload
  //"setters"
  void setFrequency(float newFrequency);
  void setPhase(float newPhase);
  void setAmplitude(float newAmplitude);
  //"getters"
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