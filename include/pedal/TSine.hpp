#ifndef TSine_hpp
#define TSine_hpp

#include <stdio.h> //@KEE this isn't needed right?
#define _USE_MATH_DEFINES  //so we can use M_PI
#include <math.h> //so we can use sin()
#include "pdlSettings.hpp"//so we can access sampleRate and bufferSize

class pdlTSine {//Pedal Trivial Sine Oscillator
  public://everything listed after this is public
  pdlTSine();//constructor, defined in the cpp
  pdlTSine(float frequency);//option to set frequency on construction
  ~pdlTSine();//deconstructor (may be needed to free memory)
  float generateSample();//generate and return a single sample
  float* generateBlock();//generate and return a block of samples

  //"setters"
  void setFrequency(float newFrequency);
  void setPhase(float newPhase);
  void setAmplitude(float newAmplitude);

  //"getters"
  float getFrequency();
  float getPhase();
  float getAmplitude();
  float getSample();
  float* getBlock();
    
  private://everything after this is private (cannot be accessed externally without
  //a "getter" or a "setter"
  //best practice to leave inner workings private

  inline float generateNextSample(){//return a float even if you don't use it
    //inline functions must be located in the header file, so 
    //we will define it here. 
    currentSample = sin(phase) * amplitude;//calculate single sample
    phase += phaseIncrement;//increment phase for the next sample
  }

  float frequency, phase, amplitude;//standard oscillator variables
  float currentSample;//current working sample
  float* currentBlock;//current working block of samples
  double phaseIncrement;//extra precision necessary 
};

#endif 