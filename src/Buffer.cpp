#include "pedal/Buffer.hpp"
//Constructors and deconstructors=====================
Buffer::Buffer(float initialDuration){
  setDuration(initialDuration);  
}
Buffer::~Buffer(){
  delete[] content;
}
//Core functionality of class=========================
void Buffer::writeSample(float inputSample, int index){
    //index = clamp(inputSample, 0, sizeInSamples-1);
    index = clamp(index, 0, sizeInSamples-1);
    content[index] = inputSample;
}

void Buffer::addToSample(float inputSample, int index){
  index = clamp(index, 0, sizeInSamples-1);
  content[index] += inputSample;
}

float Buffer::getSample(float index){
  index = clamp(index, 0.0f, sizeInSamples-1);//clamp for safety
  float retrievedSample = 0.0f;//start with a sample
  //do some linear interpolation
  float previousSample = content[int(index)];// take the 'floor' 
  //grab the next sample. wrap for edge cases
  float nextSample = content[int(index+1.0f)%sizeInSamples];// take the 'ceiling'
  //interpolate between the samples
  retrievedSample = linearInterpolation(index, previousSample, nextSample);
  return retrievedSample;
}
//getters and setters================================
void Buffer::setDuration(float newDuration){
  duration = newDuration;
  delete[] content;
  sizeInSamples = msToSamples(duration);
  content = new float[sizeInSamples];
  
  for(int i = 0; i < sizeInSamples; i++){
    content[i] = 0.0f;
  }
}
void Buffer::setSizeInSamples(int newSizeInSamples){
    sizeInSamples = newSizeInSamples;
    duration = samplesToMS(sizeInSamples);
}
float Buffer::getDuration(){return duration;}
float* Buffer::getContent(){return content;}
int Buffer::getSizeInSamples(){return sizeInSamples;}