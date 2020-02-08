#ifndef TSine_hpp
#define TSine_hpp

#include <cmath> //so we can use sin()
#include "pdlSettings.hpp"//so we can access sampleRate and bufferSize

class TSine {//Pedal Trivial Sine Oscillator
  public://everything listed after this is public
  TSine();//constructor, defined in the cpp
  TSine(float frequency);//option to set frequency on construction
  ~TSine();//deconstructor (may be needed to free memory)
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
    currentSample = std::sin(phase) * amplitude;//calculate single sample
    phase += (float)phaseIncrement;//increment phase for the next sample
    phase = fmod(phase, 6.283185307179586f);
    return currentSample;
  }

  float frequency, phase, amplitude;//standard oscillator variables
  float currentSample;//current working sample
  float* currentBlock = nullptr;//current working block of samples
  double phaseIncrement;//extra precision necessary 
};
#endif 
