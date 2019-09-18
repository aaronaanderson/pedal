#include "TSine.hpp"

//constructors and deconstructors
//=========================================================
pdlTSine::pdlTSine(){//default constructor
  setFrequency(440);
  setPhase(0.0);
  setAmplitude(1.0);
}

pdlTSine::pdlTSine(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
}

pdlTSine::~pdlTSine(){//when object is deleted
  if(currentBlock != nullptr){//if this memory was allocated
    delete currentBlock;//free the memory
  }
}

//primary mechanics of class
//=========================================================
float pdlTSine::generateSample(){//generate and return single sample
  currentSample = generateNextSample();//store the sample
  return currentSample;//return a copy
}

float* pdlTSine::generateBlock(){//it is best to do all 
  //calculations in a row if possible. This keeps the memory from 
  //jumping around looking for data

  if(currentBlock != nullptr){//if we don't have a local currentBlock yet, 
    currentBlock = new float[pdlSettings::bufferSize];//create a new array of floats
  }

  for(int i = 0; i < pdlSettings::bufferSize; ++i){//for every sample in the buffer
    currentBlock[i] = generateNextSample();
  }
  return currentBlock;//returns pointer to the begining of this block
}

//Getters and setters
//=========================================================
void pdlTSine::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency * 2.0 * M_PI)/pdlSettings::sampleRate;//*see notes on bottom
}
void pdlTSine::setPhase(float newPhase){phase = newPhase;}
void pdlTSine::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float pdlTSine::getFrequency(){return frequency;}
float pdlTSine::getAmplitude(){return amplitude;}
float pdlTSine::getSample(){return currentSample;}
float* pdlTSine::getBlock(){return currentBlock;}

/*
*Why is a phase increment needed? 

For a 1.0 Hz sinusoid, one could simply calculate the samples as a function of time
sin(time * TWO_PI);
For every second, the sinusoid will make a full rotation (phase is 0 to two_pi; at one
second, sin(1.0 * TWO_PI) is the same as sin(0.0 * TWO_PI)). Replace that 1.0 with 
whatever frequency you desire and it may seem like it should work. 

However, in digital audio samples are not calculated as needed, but are calculated slightly ahead
of time in a group of samples. This group of samples is often called a buffer, a block, or 
in the case of MaxMSP, a "signal vector". Since we have discretized time into samples, and 
we know how many samples exist per second AND how many samples we have to calculate in one group, 
we can calculate how much further the phase must travel in the next sample. This value is called
phaseIncrement.
*/