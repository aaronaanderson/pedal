#include "pedal/Buffer.hpp"

Buffer::Buffer(float initialDuration){
  duration = initialDuration;
  sizeInSamples = msToSamples(duration);
  content = new float[sizeInSamples];
}
Buffer::~Buffer(){
}

void Buffer::writeSample(float inputSample, int index){
    index = clamp(inputSample, 0, sizeInSamples);
    content[index] = inputSample;
}

float Buffer::getSample(float index){
  index = clamp(index, 0.0f, duration);//clamp for safety
  float retrievedSample = 0.0f;//start with a sample
  if(fmod(index, 1.0f) == 0.0f){//if it's an integer
    retrievedSample = content[int(index)];//straight table lookup
  }else{//if the index is a float
    //do some linear interpolation
    float previousSample = content[int(index)];// take the 'floor' 
    //grab the next sample. wrap for edge cases
    float nextSample = content[int(index+1.0f)%sizeInSamples];
    //interpolate between the samples
    retrievedSample = linearInterpolation(index, previousSample, nextSample);
  }
  return retrievedSample;
}

void Buffer::setDuration(float newDuration){
  if(newDuration != duration){
    duration = newDuration;
    delete[] content;
    sizeInSamples = msToSamples(duration);
    content = new float[sizeInSamples];
  }
}
void Buffer::setSizeInSamples(int newSizeInSamples){
    sizeInSamples = newSizeInSamples;
    duration = samplesToMS(sizeInSamples);
}
float Buffer::getDuration(){return duration;}
float* Buffer::getContent(){return content;}
int Buffer::getSizeInSamples(){return sizeInSamples;}