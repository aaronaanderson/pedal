#include "pedal/TSquare.hpp"

//constructors and deconstructors
//=========================================================
TSquare::TSquare(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
  setAmplitude(1.0);
  setDutyCycle(0.5);
}

TSquare::TSquare(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
  setDutyCycle(0.5);
}

TSquare::~TSquare(){//deconstructor (needed to be explicit if freeing memory)
  if(currentBlock != nullptr){//if space was allocated for current
    delete[] currentBlock;//free the memory
  }
}

//primary mechanics of class
//=========================================================
float TSquare::generateSample(){//return a float even if you don't use it
  phase += phaseIncrement;
  while(phase > 1.0){
    phase -= 1.0;
  }
  while(phase < 0.0){
      phase += 1.0;
  }

  if(phase > dutyCycle){
    currentSample = -1.0;
  }else{
    currentSample = 1.0;
  }
  return currentSample;
}

float* TSquare::generateBlock(){//it is best to do all 
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
void TSquare::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency)/pdlSettings::sampleRate;
}
void TSquare::setPhase(float newPhase){//expecting (0-2PI)
    phase = fmod(newPhase, 2.0 * 3.1415926);//ensure 0-2PI
    phase -= 3.1415926;//now -PI to PI
    phase /= 3.1415926;//nown -1 to 1
    //we need an offset. 0.0 phase should be 0.0 result
}
void TSquare::setDutyCycle(float newDutyCycle){dutyCycle = newDutyCycle;}
void TSquare::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float TSquare::getFrequency(){return frequency;}
float TSquare::getAmplitude(){return amplitude;}
float TSquare::getSample(){return currentSample;}
float* TSquare::getBlock(){return currentBlock;}
