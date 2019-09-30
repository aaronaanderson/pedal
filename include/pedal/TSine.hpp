#ifndef TSine_hpp
#define TSine_hpp

#include <stdio.h> //@KEE this isn't needed right? >> yes, not needed
#define _USE_MATH_DEFINES  //so we can use M_PI >> this is MCVC only. need to have custom constant
#include <math.h> //so we can use sin()
#include "pdlSettings.hpp"//so we can access sampleRate and bufferSize >> only used in cpp file, not needed here

class pdlTSine {//Pedal Trivial Sine Oscillator
public://everything listed after this is public
  pdlTSine();//constructor, defined in the cpp
  pdlTSine(float frequency);//option to set frequency on construction

  // memory is allocated in this class, options are:
  // 1. use managed memory - std::vector
  // 2. write all the components of Rule of 5
  //    ref) https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming)
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

  // is it necessary to make this inline?
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
