#ifndef Buffer_hpp
#define Buffer_hpp

#include "utilities.hpp"

class Buffer{
  public:
  Buffer(float initialSize = 1000.0f);
  ~Buffer();
  
  void writeSample(float inputSample, int index);

  void setDuration(float newDuration);
  void setSizeInSamples(int newSizeInSamples);

  float getSample(int index);
  float* getContent();
  int getSizeInSamples();

  private:
  float* content;
  int sizeInSamples;
  float duration;
};
#endif