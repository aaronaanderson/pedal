#include "pedal/BufferTap.hpp"

BufferTap::BufferTap(CircularBuffer* reference){
  bufferReference = reference;
  setDelayTime(100.0f);//100ms behind write point
}
BufferTap::~BufferTap(){
  delete[] currentBlock;
}

float BufferTap::getSample(){
  currentSample = bufferReference->getDelayed(delayTime);
  return currentSample;
}

float* BufferTap::getBlock(){
  if(currentBlock == nullptr){
    currentBlock = new float[pdlSettings::bufferSize];
  }
  for(int i = 0; i < pdlSettings::bufferSize; i++){
    currentBlock[i] = getSample();
  }
  return currentBlock;
}

void BufferTap::setDelayTime(float newDelayTime){
  delayTime = clamp(newDelayTime, 
                    0.0f, 
                    bufferReference->getDuration());
}
void BufferTap::setReference(CircularBuffer* newReference){bufferReference = newReference;}