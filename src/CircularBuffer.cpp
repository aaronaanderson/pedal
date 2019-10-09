#include "pedal/CircularBuffer.hpp"

CircularBuffer::CircularBuffer(float initialDuration){
  buffer.setDuration(initialDuration);
  writeLocation = 0;
}
void CircularBuffer::inputSample(float inputSample){
  buffer.writeSample(inputSample, writeLocation);
  writeLocation++;
  if(writeLocation >= buffer.getSizeInSamples()){
    writeLocation = 0;
  }
}
float CircularBuffer::getDelayed(float timeBack){
  timeBack = clamp(timeBack, 0.0f, buffer.getDuration());
  float samplesToLookBack = msToSamples(timeBack);
  samplesToLookBack = writeLocation - samplesToLookBack;
  samplesToLookBack = fmod(samplesToLookBack + buffer.getSizeInSamples(), 
                           buffer.getSizeInSamples());
  return buffer.getSample(samplesToLookBack);//buffer automatically interpolates
}
void CircularBuffer::inputBlock(float* inputBlock){
  //is there a way to confirm the incoming block is valid?
  for(int i = 0; i < pdlSettings::bufferSize; i++){
    inputSample(inputBlock[i]);
  }
}
void CircularBuffer::setSizeInSamples(int newSizeInSamples){
  buffer.setDuration(samplesToMS(newSizeInSamples));
}
void CircularBuffer::setDuration(float newDuration){
  buffer.setDuration(newDuration);
}
float CircularBuffer::getDuration(){return buffer.getDuration();}
int CircularBuffer::getSizeInSamples(){return buffer.getSizeInSamples();}