#ifndef TapIn_hpp
#define TapIn_hpp

#include "pdlSettings.hpp"

class TapIn{
  public:
  //TapIn();
  TapIn(float initialSize = 1000.0f);
  ~TapIn();//willl need to free memory
  
  void inputSample(float inputSample);//input a sample and automatically progress buffer
  void inputBlock(float* inputBlock);//process a block at a time
  
  void setBufferSizeMS(float newSize);
  void setBufferSizeSamples(float newSizeInSamples);
  
  float getSample(float timeBack = 0.0f);//give most recent sample by default
  float* getBuffer();//retrieve a pointer to the entire buffer
  int getBufferSize();

  private:
  float* buffer;//a (pointer to a) buffer (array) of floats 
  float bufferLocation;//location within the buffer
  int bufferSize;
};
#endif 