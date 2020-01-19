#include "pedal/Buffer.hpp"
#include <cstring> // memcpy

//Constructors and deconstructors=====================
// -----rule of 5 --------------------
Buffer::Buffer(float initialDuration){
  setDuration(initialDuration);  
}

Buffer::Buffer(const Buffer& other) {
  duration = other.duration;
  sizeInSamples = other.sizeInSamples;
  content = new float[sizeInSamples];
  std::memcpy(content, other.content, sizeInSamples * sizeof(float));
}

Buffer::Buffer(Buffer&& other) noexcept {
  duration = other.duration;
  sizeInSamples = other.sizeInSamples;
  content = other.content;
  other.content = nullptr;
}

Buffer& Buffer::operator=(const Buffer& other) {
  duration = other.duration;
  sizeInSamples = other.sizeInSamples;
  delete[] content; // delete what we have
  content = new float[sizeInSamples];
  std::memcpy(content, other.content, sizeInSamples * sizeof(float));
  return *this;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
  if (this != &other) { // check self assignment
    duration = other.duration;
    sizeInSamples = other.sizeInSamples;
    delete[] content; // delete what we have
    content = other.content;
    other.content = nullptr;
  }
  return *this;
}
//-----------------------------------
Buffer::~Buffer(){
  delete[] content;
}

//Core functionality of class=========================
void Buffer::writeSample(float inputSample, int index){
  // TODO !!!!!!!!!!!!!!!!!!!! clamp works on floats!
  // Use templated clamp for have separate function for ints

  index = clamp(index, 0, sizeInSamples-1);
  content[index] = inputSample;
}

void Buffer::addToSample(float inputSample, int index){
  // TODO !!!!!!!!!!!!!!!!!!!! clamp works on floats!
  // use templated clamp for have separate function for ints

  index = clamp(index, 0, sizeInSamples-1);
  content[index] += inputSample;
}

float Buffer::getSample(float index){
#if 0
  index = clamp(index, 0.0f, sizeInSamples-1);//clamp for safety
  float retrievedSample = 0.0f;//start with a sample
  //do some linear interpolation
  float previousSample = content[int(index)];// take the 'floor' 
  //grab the next sample. wrap for edge cases
  float nextSample = content[int(index+1.0f)%sizeInSamples];// take the 'ceiling'
  //interpolate between the samples
  retrievedSample = linearInterpolation(index, previousSample, nextSample);
  return retrievedSample;
#else

  if (index < 0.0f) index = 0.0f; // TODO? maybe negative index for wrapping backwards? for now we clamp
  int i0 = (int)index;  // index for previous sample
  int i1 = i0 + 1;      // index for next sample
  float t = index - i0; // interpolation amount

  // wrap indices
  i0 = i0 % sizeInSamples;
  i1 = i1 % sizeInSamples;

  return linearInterpolation(t, content[i0], content[i1]);
#endif
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