#ifndef TrivialSine_hpp
#define TrivialSine_hpp

#include "utilities.hpp"
#include "Constants.hpp"
#include <cmath> //so we can use sin()
#include "Settings.hpp"//so we can access sampleRate and bufferSize

namespace pedal{
class TrivialSine {//Pedal Trivial Sine Oscillator
  public://everything listed after this is public
  TrivialSine();//constructor, defined in the cpp
  TrivialSine(float frequency);//option to set frequency on construction
  ~TrivialSine();//deconstructor (may be needed to free memory)
  float generateSample();//generate and return a single sample
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
    
  private://everything after this is private

  float frequency;
  float phase;
  float amplitude;
  float currentSample;//current working sample
  double phaseIncrement;//extra precision necessary 
};
}//end pedal namespace
#endif 
