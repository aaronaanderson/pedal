#ifndef Buffer_hpp
#define Buffer_hpp

#include "utilities.hpp"
#include "Interpolation.hpp"

class Buffer{
  public:
  Buffer(float initialDuration = 1000.0f);
  ~Buffer();
  
  void writeSample(float inputSample, int index);

  void setDuration(float newDuration);
  void setSizeInSamples(int newSizeInSamples);

  float getSample(float index);
  float* getContent();
  float getDuration();
  int getSizeInSamples();
  
  private:
  float* content;
  int sizeInSamples;
  float duration;
};
/*
TODO make buffer N channel
*/
#endif