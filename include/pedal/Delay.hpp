#ifndef Delay_hpp
#define Delay_hpp

#include "CircularBuffer.hpp"//Circular Buffers are ideal for delay
#include "BufferTap.hpp"//an automated buffer lookup for CircularBuffer
#include "pdlSettings.hpp"//access to pdlSettings::bufferSize

class Delay{
  public:
  Delay();//default constructor
  Delay(float initialTime, float initialFeedback);//override constructor
  ~Delay();//free memory
  
  float insertSample(float inputSample);
 
  void setDelayTime(float newDelayTime);
  void setFeedback(float newFeedback);
  void setMaximumFeedbackTime(float newMaxFeedbackTime);
  float getDelayTime();
  float getFeedback();
  float getMaximumFeedbackTime();
  float getSample();//retrieve currentSample w/o processing

  private:
  float currentSample;
  CircularBuffer buffer;//storage for past samples 
  BufferTap* tap;//@kee remind me of question here
  float feedback;//how much of the feed back is re-inserted
  float delayTime;//Samples from 'delayTime' milliseconds ago
};
#endif