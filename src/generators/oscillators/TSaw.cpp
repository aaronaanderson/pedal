
#include "pedal/TSaw.hpp"

//constructors and deconstructors
//=========================================================
TSaw::TSaw(){//default constructor
  setFrequency(440);//default frequency is 440
  setPhase(0.0);
  setAmplitude(1.0);
}

TSaw::TSaw(float frequency){//override constructor
  setFrequency(frequency);
  setPhase(0.0);
  setAmplitude(1.0);
}

TSaw::~TSaw(){//deconstructor (needed to be explicit if freeing memory)
  if(currentBlock != nullptr){//if space was allocated for current
    delete[] currentBlock;//free the memory
  }
}

//primary mechanics of class
//=========================================================
float TSaw::generateSample(){//return a float even if you don't use it
  phase += (float)phaseIncrement;
  while(phase > 1.0){
    phase -= 2.0;
  }
   while(phase < -1.0){//to ensure that negative frequencies will work
    phase += 2.0;
  }
  currentSample = phase * amplitude;
  return currentSample;
}

float* TSaw::generateBlock(){//it is best to do all 
  //calculations in a row if possible. This keeps the memory from 
  //jumping around looking for data

  if(currentBlock != nullptr){//if we don't have a local currentBlock yet, 
    currentBlock = new float[pdlSettings::bufferSize];//create a new array of floats
  }

  for(int i = 0; i < pdlSettings::bufferSize; ++i){//for every sample in the buffer
    currentBlock[i] = generateSample();
  }
  return currentBlock;//returns pointer to the begining of this block
}
//Getters and setters
//=========================================================
void TSaw::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = (frequency * 2.0)/pdlSettings::sampleRate;
}
void TSaw::setPhase(float newPhase){//set phase (0 - 2PI)
  //convert the standard 0-2PI range to -1 to 1 to make calculations cheap
  phase = fmod(newPhase, 2.0f * 3.1415926535f);//make sure phase is in the 0-twopi range
  phase -= 3.1415926535f;//now the range is (-pi, pi);
  phase /= 3.1415926535f;//now the phase is (-1, 1);
}
void TSaw::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float TSaw::getFrequency(){return frequency;}
float TSaw::getAmplitude(){return amplitude;}
float TSaw::getSample(){return currentSample;}
float* TSaw::getBlock(){return currentBlock;}
