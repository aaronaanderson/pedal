
#include "TTriangle.hpp"

//constructors and deconstructors
//=========================================================
pdlTTriangle::pdlTTriangle(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
  setAmplitude(1.0);
}

pdlTTriangle::pdlTTriangle(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
}

pdlTTriangle::~pdlTTriangle(){//deconstructor (needed to be explicit if freeing memory)
  if(currentBlock != nullptr){//if space was allocated for current
    delete currentBlock;//free the memory
  }
}

//primary mechanics of class
//=========================================================
float pdlTTriangle::generateSample(){//return a float even if you don't use it
  currentSample = generateNextSample();//defined in header (b/c inlined function)
  return currentSample;
}

float* pdlTTriangle::generateBlock(){//it is best to do all 
  //calculations in a row if possible. This keeps the memory from 
  //jumping around looking for data

  if(currentBlock != nullptr){//if we don't have a local currentBlock yet, 
    currentBlock = new float[pdlSettings::bufferSize];//create a new array of floats
  }

  for(int i = 0; i < pdlSettings::bufferSize; ++i){//for every sample in the buffer
    currentBlock[i] = generateNextSample();//place the calculated sample at current index
  }
  return currentBlock;//returns pointer to the begining of this block
}
//Getters and setters
//=========================================================
void pdlTTriangle::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency * 2.0)/pdlSettings::sampleRate;
}
void pdlTTriangle::setPhase(float newPhase){//expecting (0-2PI)
    phase = fmod(newPhase, 2.0 * M_PI);//ensure 0-2PI
    phase -= M_PI;//now -PI to PI
    phase /= M_PI;//nown -1 to 1
    //we need an offset. 0.0 phase should be 0.0 result
}
void pdlTTriangle::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float pdlTTriangle::getFrequency(){return frequency;}
float pdlTTriangle::getAmplitude(){return amplitude;}
float pdlTTriangle::getSample(){return currentSample;}
float* pdlTTriangle::getBlock(){return currentBlock;}
