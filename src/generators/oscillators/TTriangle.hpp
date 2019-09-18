
#ifndef TTriangle_hpp
#define TTriangle_hpp

#include <stdio.h>
#define _USE_MATH_DEFINES  //so we can use M_PI
#include <math.h> //so we can use sin()
#include "../../pdlSettings.hpp"//so we can access sampleRate and bufferSize

class pdlTTriangle{//Pedal Trivial Sine Oscillator
  public:
  pdlTTriangle();//constructor, defined in the cpp
  pdlTTriangle(float frequency);//option to set frequency on construction
  ~pdlTTriangle();//deconstructor (may be needed to free memory)
  float generateSample();//calculate and return next sample
  float* generateBlock();//calculate and return next block of samples

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
    
    //=======================================================
    private://best practice to leave inner workings private
    inline float generateNextSample(){
      //We can start by making a trivial sawtooth
      phase += phaseIncrement;
      while(phase > 1.0){
        phase -= 1.0;
      }
      while(phase < 0.0){//to ensure that negative frequencies will work
        phase += 1.0;
      }
      //then convert sawtooth to triangle by flipping the negative regions
      currentSample = (fabs(phase) * 2.0) - 1.0;//convert from saw to triangle
      currentSample *= amplitude;//scale by
    }

    float frequency, phase, amplitude;//standard oscillator variables
    float currentSample;//current working sample
    float* currentBlock;//current working block of samples
    double phaseIncrement;//extra precision necessary 
};
#endif