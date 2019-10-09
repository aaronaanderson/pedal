#include "pedal/Buffer.hpp"

Buffer::Buffer(float initialSize = 1000.0f){
  duration = initialSize;
  sizeInSamples = msToSamples(initialSize);
  content = new float[sizeInSamples];
}
Buffer::~Buffer(){
  delete[] content;
}

void Buffer::writeSample(float inputSample, int index){
    index = clamp(inputSample, 0, sizeInSamps);
    content[index] = inputSample;
}

void Buffer::setDuration(float newDuration){
  if(newDuration != duration){
    duration = newDuration;
    delete[] content;
    sizeInSamps = msToSamples(duration);
    content = new float[sizeInSamps];
  }
}
void Buffer::setSizeInSamples(int newSizeInSamples){
    sizeInSamples = newSizeInSamples;
    duration = msFromSamples
}

float* Buffer::getContent(){return content;}
int Buffer::getSizeInSamps(){return sizeInSamps;}