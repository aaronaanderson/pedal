#ifndef BufferTap_hpp
#define BufferTap_hpp

#include "CircularBuffer.hpp"
#include <cmath>

namespace pedal{
class BufferTap{
  public:
  BufferTap(CircularBuffer* reference);
  ~BufferTap();

  void setDelayTime(float newDelayTime);
  void setReference(CircularBuffer* newReference);
  float getSample();

  private:
  CircularBuffer* bufferReference;
  float delayTime;
  float readIndex;
  float currentSample;
};
}//end pedal namespace
#endif