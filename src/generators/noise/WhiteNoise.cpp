#include "pedal/WhiteNoise.hpp"

//constructors and deconstructors
//=========================================================
WhiteNoise::WhiteNoise(){
  currentSample = generateSample();//start wtih a random sample
}
WhiteNoise::~WhiteNoise(){
  delete[] currentBlock;
}

//primary mechanics of class
//=========================================================
float WhiteNoise::generateSample(){//generate a single sample
  currentSample = rangedRandom(-1.0f, 1.0f);//very basic random....could be more random
  return currentSample;
}
float* WhiteNoise::generateBlock(){//generate a block of samples
  if(currentBlock != nullptr){//if we don't have a local currentBlock yet, 
    currentBlock = new float[pdlSettings::bufferSize];//create a new array of floats
  }
  for(int i = 0; i < pdlSettings::bufferSize; i++){//for every index in the buffer
    currentBlock[i] = generateSample();//generate and assign a single sample
  }
  return currentBlock;
}

//Getters and setters
//=========================================================
float WhiteNoise::getSample(){return currentSample;}
float* WhiteNoise::getBlock(){return currentBlock;}