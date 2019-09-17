#ifndef TSine_hpp
#define TSine_hpp

#include <stdio.h>
#define _USE_MATH_DEFINES  //so we can use M_PI
#include <math.h> //so we can use sin()
#include "../../pdlSettings.hpp"//so we can access sampleRate and bufferSize

class pdlTSine {//Pedal Trivial Sine Oscillator
    public:
    pdlTSine();//constructor, defined in the cpp
    pdlTSine(float frequency);//option to set frequency on construction
    
    inline float generate(){//return a float even if you don't use it
        //inline functions must be located in the header file, so 
        //we will define it here. 

        currentSample = sin(phase) * amplitude;//calculate single sample
        phase += phaseIncrement;//increment phase for the next sample
    }

    inline float* generateBlock(){//it is best to do all 
        //calculations in a row if possible. This keeps the memory from 
        //jumping around looking for data

        if(currentBlock != nullptr){//if we don't have a local currentBlock yet, 
            currentBlock = new float[pdlSettings::bufferSize];//create a new array of floats
        }

        for(int i = 0; i < pdlSettings::bufferSize; ++i){//for every sample in the buffer
            currentBlock[i] = sin(phase) * amplitude;//calculate the next sample
            phase += phaseIncrement;//increment phase
        }
        return currentBlock;//returns pointer to the begining of this block
    }

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
    
    private://best practice to leave inner workings private
    float frequency, phase, amplitude;//standard oscillator variables
    float currentSample;//current working sample
    float* currentBlock;//current working block of samples
    double phaseIncrement;//extra precision necessary 
}

#endif 