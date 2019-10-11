#ifndef Delay_hpp
#define Delay_hpp

#include "CircularBuffer.hpp"
#include "BufferTap.hpp"
#include "pdlSettings.hpp"

class Delay{
  public:
  Delay();
  ~Delay();
  
  float getSample(float inputSample);
  float* getBlock(float* inputBlock);
 
  void setFeedback(float newFeedback);
  float getSample();
  float* getBlock();
  
  private:
  float currentSample;
  float* currentBlock;
  CircularBuffer buffer;
  BufferTap* tap;//@kee remind me of question here
  float feedback;
  float delayTime;
};
#endif