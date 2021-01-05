#ifndef TSquare_hpp
#define TSquare_hpp

#include "pdlConstants.hpp"
#include <cmath> //so we can use sin()
#include "pdlSettings.hpp"//so we can access sampleRate and bufferSize

namespace pedal{
class TSquare{//Pedal Trivial Square Oscillator
  public:
  TSquare();//constructor, defined in the cpp
  TSquare(float frequency);//option to set frequency on construction
  ~TSquare();//deconstructor (may be needed to free memory)
  float generateSample();//calculate and return next sample

  //"setters"
  void setFrequency(float newFrequency);
  void setPhase(float newPhase);
  void setAmplitude(float newAmplitude);
  void setDutyCycle(float newDutyCycle);

  //"getters"
  float getFrequency();
  float getPhase();
  float getAmplitude();
  float getSample();
  float getDutyCycle();

  //=======================================================
  private://best practice to leave inner workings private
  float frequency, phase, amplitude;//standard oscillator variables
  float dutyCycle;//width of rectangular wave (square if 0.5);
  float currentSample;//current working sample
  double phaseIncrement;//extra precision necessary 
};
}//end pedal namespace
#endif