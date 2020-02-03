
#ifndef TTriangle_hpp
#define TTriangle_hpp

#include <math.h> //so we can use sin()
#include "pdlSettings.hpp"//so we can access sampleRate and bufferSize

class TTriangle{//Pedal Trivial Sine Oscillator
  public:
  TTriangle();//constructor, defined in the cpp
  TTriangle(float frequency);//option to set frequency on construction
  ~TTriangle();//deconstructor (may be needed to free memory)
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
    float frequency, phase, amplitude;//standard oscillator variables
    float currentSample;//current working sample
    float* currentBlock = nullptr;//current working block of samples
    double phaseIncrement;//extra precision necessary 
};
#endif