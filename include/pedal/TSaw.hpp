#ifndef TSaw_hpp
#define TSaw_hpp

#include "pdlConstants.hpp"
#include <cmath> //so we can use sin()
#include "pdlSettings.hpp"//so we can access sampleRate and bufferSize

namespace pedal{
class TSaw {//Trivial Saw Oscillator
  public:
  TSaw();//constructor, defined in the cpp
  TSaw(float frequency);//option to set frequency on construction
  ~TSaw();//deconstructor (may be needed to free memory)
  float generateSample();//calculate and return next sample

    //"setters"
    void setFrequency(float newFrequency);
    void setPhase(float newPhase);
    void setAmplitude(float newAmplitude);

    //"getters"
    float getFrequency();
    float getPhase();
    float getAmplitude();
    float getSample();
    
    //=======================================================
    private://best practice to leave inner workings private
  
    float frequency, phase, amplitude;//standard oscillator variables
    float currentSample;//current working sample
    double phaseIncrement;//extra precision necessary 
};
}//end pedal namespace
#endif 