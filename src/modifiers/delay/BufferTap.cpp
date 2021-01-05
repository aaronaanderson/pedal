#include "pedal/BufferTap.hpp"

using namespace pedal;

BufferTap::BufferTap(CircularBuffer* reference){
  bufferReference = reference;
  setDelayTime(100.0f);//100ms behind write point
}
BufferTap::~BufferTap(){}//when object is deleted

float BufferTap::getSample(){
  currentSample = bufferReference->getDelayed(delayTime);
  return currentSample;
}

void BufferTap::setDelayTime(float newDelayTime){
  delayTime = clamp(newDelayTime, 
                    0.0f, 
                    bufferReference->getDuration());
}
void BufferTap::setReference(CircularBuffer* newReference){bufferReference = newReference;}