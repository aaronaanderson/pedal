#include "pedal/CircularBuffer.hpp"
#include "pedal/DebugTool.hpp"
CircularBuffer::CircularBuffer(float initialDuration){
  buffer.setDuration(initialDuration);//request space in ram
  writeLocation = 0;//start the write index at the beginning
}
void CircularBuffer::inputSample(float inputSample){
  buffer.writeSample(inputSample, writeLocation);
  writeLocation++;// increment the write index
  if(writeLocation >= buffer.getDurationInSamples()){
    writeLocation = 0;
  }
}
float CircularBuffer::getDelayed(float timeBack){
  timeBack = clamp(timeBack, 0.0f, buffer.getDuration()); // clamp to 0 .. bufferDuration
  float samplesBack = msToSamples(timeBack); // convert ms to samples
  float index = writeLocation - samplesBack; // sample index location that could be negative
  if(index < 0.0f){index += buffer.getDurationInSamples();}
  float previousSample = buffer.getSample((int)index);
  //increment and wrap index
  index = ((int)index+1) % buffer.getDurationInSamples();
  float nextSample = buffer.getSample(index);
  float interpolatedResult = linearInterpolation(samplesBack, previousSample, nextSample);
  return interpolatedResult;//buffer automatically interpolates
}
void CircularBuffer::inputBlock(float* inputBlock){
  //is there a way to confirm the incoming block is valid?
  for(int i = 0; i < pdlSettings::bufferSize; i++){
    inputSample(inputBlock[i]);
  }
}
void CircularBuffer::setDuration(float newDuration){
  buffer.setDuration(newDuration);
}
void CircularBuffer::setDurationInSamples(int newDurationInSamples){
  buffer.setDuration(samplesToMS(newDurationInSamples));
}
float* CircularBuffer::getBuffer(){return buffer.getContent();}
float CircularBuffer::getDuration(){return buffer.getDuration();}
int CircularBuffer::getDurationInSamples(){return buffer.getDurationInSamples();}
int CircularBuffer::getWriteLocation(){return writeLocation;}
Buffer* CircularBuffer::getBufferReference(){return &buffer;}