#ifndef BufferTap_hpp
#define BufferTap_hpp

#include "CircularBuffer.hpp"
#include "math.h"

class BufferTap{
  public:
  BufferTap(CircularBuffer* reference);
  ~BufferTap();

  void setDelayTime(float newDelayTime);
  void setReference(CircularBuffer* newReference);
  float getSample();
  float* getBlock();

  private:
  CircularBuffer* bufferReference;
  float delayTime;
  float readIndex;
  float currentSample;
  float* currentBlock = nullptr;
};
#endif