#ifndef TrivialSquare_hpp
#define TrivialSquare_hpp

#include "pdlConstants.hpp"
#include <cmath> //so we can use sin()
#include "Settings.hpp"//so we can access sampleRate and bufferSize

namespace pedal{
class TrivialSquare{//Pedal Trivial Square Oscillator
  public:
  TrivialSquare();//constructor, defined in the cpp
  TrivialSquare(float frequency);//option to set frequency on construction
  ~TrivialSquare();//deconstructor (may be needed to free memory)
  float generateSample();//calculate and return next sample
  float generateSample(float fromPhase);//phase driven overload
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