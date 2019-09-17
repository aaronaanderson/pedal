//#ifndef TrivialOscillators_hpp
//#define TrivialOscillators_hpp

#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

//#endif /* TrivialOscillators_hpp */

class pdlTSine {//Pedal Trivial Sine Oscillator
    public:
    pdlTSine();//constructor, defined in the cpp

    inline float generate(){//return a float even if you don't use it
        //inline functions must be located in the header file, so 
        //we will define it here. 

        currentSample = sin(phase) * amplitude;
        phase += phaseIncrement;
    }

    inline float* generateBlock(){//it is best to do all 
        //calculations in a row if possible. This keeps the memory from 
        //jumping around looking for data

        if(currentBlock != nullptr){//if we don't have a local currentBlock yet, 
            currentBlock = new float[pdlSettings::bufferSize];
        }
        for(int i = 0; i < pdlSettings::bufferSize; ++i){
            currentBlock[i] = sin(phase) * amplitude;
            phase += phaseIncrement;
        }
        return currentBlock;
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

