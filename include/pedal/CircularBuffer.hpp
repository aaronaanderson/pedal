#ifndef CircularBuffer_hpp
#define CircularBuffer_hpp

#include "pdlSettings.hpp"
#include "utilities.hpp"
#include "Buffer.hpp"

class CircularBuffer{
  public:
  //TapIn();
  CircularBuffer(float initialSize = 1000.0f);
  //~CircularBuffer();//willl need to free memory
  
  void inputSample(float inputSample);//input a sample and automatically progress buffer
  void inputBlock(float* inputBlock);//process a block at a time
  
  void setBufferSize(float newSizeInSamples);
  void setDuration(float newSize);
  void setBufferSizeSamples(float newSizeInSamples);
  
  float getSample(float timeBack = 0.0f);//give most recent sample by default
  float* getBuffer();//retrieve a pointer to the entire buffer
  int getBufferSize();

  private:
  Buffer buffer;//buffer in which to store audio
  int writeLocation;//location within the buffer
};
#endif 