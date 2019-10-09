#ifndef Tap_hpp
#define Tap_hpp

#include "CircularBuffer.hpp"
#include "math.h"

class BufferTap{
  public:
  BufferTap(CircularBuffer* reference);
  ~BufferTap();

  void setDelayTime(float newDelayTime);
  float getSample();
  float* getBlock();

  private:
  CircularBuffer* bufferReference;
  float delayTime;
  float readIndex;
  float currentSample;
  float* currentBlock;
};
#endif