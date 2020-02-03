#ifndef TSquare_hpp
#define TSquare_hpp

#include <math.h> //so we can use sin()
#include "pdlSettings.hpp"//so we can access sampleRate and bufferSize

class TSquare{//Pedal Trivial Square Oscillator
  public:
  TSquare();//constructor, defined in the cpp
  TSquare(float frequency);//option to set frequency on construction
  ~TSquare();//deconstructor (may be needed to free memory)
  float generateSample();//calculate and return next sample
  float* generateBlock();//calculate and return next block of samples

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
  float* getBlock();
  float getDutyCycle();

  //=======================================================
  private://best practice to leave inner workings private
  float frequency, phase, amplitude;//standard oscillator variables
  float dutyCycle;//width of rectangular wave (square if 0.5);
  float currentSample;//current working sample
  float* currentBlock = nullptr;//current working block of samples
  double phaseIncrement;//extra precision necessary 
};
#endif