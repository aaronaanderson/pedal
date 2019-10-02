
#include "pedal/TTriangle.hpp"

//constructors and deconstructors
//=========================================================
TTriangle::TTriangle(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
  setAmplitude(1.0);
}

TTriangle::TTriangle(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
}

TTriangle::~TTriangle(){//deconstructor (needed to be explicit if freeing memory)
  if(currentBlock != nullptr){//if space was allocated for current
    delete[] currentBlock;//free the memory
  }
}

//primary mechanics of class
//=========================================================
float TTriangle::generateSample(){//return a float even if you don't use it
  //We can start by making a trivial sawtooth
  phase += phaseIncrement;
  while(phase > 1.0){
    phase -= 2.0;
  }
  while(phase < -1.0){//to ensure that negative frequencies will work
    phase += 2.0;
  }
  //then convert sawtooth to triangle by flipping the negative regions
  currentSample = (fabs(phase) * 2.0) - 1.0;//convert from saw to triangle
  currentSample *= amplitude;//scale by
  return currentSample;
}

float* TTriangle::generateBlock(){//it is best to do all 
  //calculations in a row if possible. This keeps the memory from 
  //jumping around looking for data

  if(currentBlock != nullptr){//if we don't have a local currentBlock yet, 
    currentBlock = new float[pdlSettings::bufferSize];//create a new array of floats
  }

  for(int i = 0; i < pdlSettings::bufferSize; ++i){//for every sample in the buffer
    currentBlock[i] = generateSample();//place the calculated sample at current index
  }
  return currentBlock;//returns pointer to the begining of this block
}
//Getters and setters
//=========================================================
void TTriangle::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency * 2.0 )/pdlSettings::sampleRate;
}
void TTriangle::setPhase(float newPhase){//expecting (0-2PI)
    phase = fmod(newPhase, 2.0 * 3.1415926);//ensure 0-2PI
    phase -= 3.1415926;//now -PI to PI
    phase /= 3.1415926;//nown -1 to 1
    //we need an offset. 0.0 phase should be 0.0 result
}
void TTriangle::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float TTriangle::getFrequency(){return frequency;}
float TTriangle::getAmplitude(){return amplitude;}
float TTriangle::getSample(){return currentSample;}
float* TTriangle::getBlock(){return currentBlock;}
