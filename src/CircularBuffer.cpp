#include "pedal/CircularBuffer.hpp"

CircularBuffer::CircularBuffer(float initialDuration = 1000.0f){
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

void CircularBuffer::inputBlock(float* inputBlock){
  //is there a way to confirm the incoming block is valid?
  for(int i = 0; i < pdlSettings::bufferSize; i++){
    inputSample(inputBlock[i]);
  }
}

void setBufferSize(int newSizeInSamples){
    buffer.setDuration(samplesToMS(newSizeInSamples));
}