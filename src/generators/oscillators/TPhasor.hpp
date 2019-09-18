#ifndef TPhasor_hpp
#define TPhasor_hpp

#include <stdio.h>
#define _USE_MATH_DEFINES  //so we can use M_PI
#include <math.h> //so we can use sin()
#include "../../pdlSettings.hpp"//so we can access sampleRate and bufferSize

class pdlTPhasor{//Pedal Trivial Sine Oscillator
  public:
  pdlTPhasor();//constructor, defined in the cpp
  pdlTPhasor(float frequency);//option to set frequency on construction
  ~pdlTPhasor();//deconstructor (may be needed to free memory)
  float generateSample();//calculate and return next sample
  float* generateBlock();//calculate and return next block of samples

    //"setters"
    inline void setFrequency(float newFrequency);
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
      phase += phaseIncrement;
      while(phase > 1.0){
        phase -= 1.0;
      }
      while(phase < 0.0){//to ensure that negative frequencies will work
        phase += 1.0;
      }
      currentSample = phase * amplitude;
    }

    float frequency, phase, amplitude;//standard oscillator variables
    float currentSample;//current working sample
    float* currentBlock;//current working block of samples
    double phaseIncrement;//extra precision necessary 
};

#endif 