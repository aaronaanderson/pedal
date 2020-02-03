#ifndef Delay_hpp
#define Delay_hpp

#include "CircularBuffer.hpp"//Circular Buffers are ideal for delay
#include "BufferTap.hpp"//an automated buffer lookup for CircularBuffer
#include "pdlSettings.hpp"//access to pdlSettings::bufferSize

class Delay{
  public:
  Delay();//default constructor
  Delay(float initialTime, float initialFeedback);//override constructor
  ~Delay();//free RAM
  
  float insertSample(float inputSample);
  float* insertBlock(float* inputBlock);
 
  void setDelayTime(float newDelayTime);
  void setFeedback(float newFeedback);
  void setMaximumFeedbackTime(float newMaxFeedbackTime);

  float getSample();//retrieve currentSample w/o processing
  float* getBlock();//retrieve currentBlock w/0 processing

  private:
  float currentSample;
  float* currentBlock = nullptr;
  CircularBuffer buffer;//storage for past samples 
  BufferTap* tap;//@kee remind me of question here
  float feedback;//how much of the feed back is re-inserted
  float delayTime;//Samples from 'delayTime' milliseconds ago
};
#endif