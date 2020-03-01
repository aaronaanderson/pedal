#ifndef CircularBuffer_hpp
#define CircularBuffer_hpp

#include "pdlSettings.hpp"
#include "utilities.hpp"
#include "Interpolation.hpp"
#include "Buffer.hpp"

class CircularBuffer{
  public:
  //TapIn();
  CircularBuffer(float initialDuration = 1000.0f);
  //~CircularBuffer();//willl need to free memory
  
  void inputSample(float inputSample);//input a sample and automatically progress buffer
  void inputBlock(float* inputBlock);//process a block at a time
  
  void setDuration(float newDuration);
  void setDurationInSamples(int newDurationInSamples);
  
  float getDelayed(float timeBack = 0.0f);//give most recent sample by default
  float* getBuffer();//retrieve a pointer to the entire buffer
  float getDuration();
  int getDurationInSamples();
  int getWriteLocation();
  Buffer* getBufferReference();

  private:
  Buffer buffer;//buffer in which to store audio
  int writeLocation;//location within the buffer
};
#endif