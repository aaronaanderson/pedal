#ifndef LowPassCombFilter_hpp
#define LowPassCombFilter_hpp

#include "pedal/CircularBuffer.hpp"
#include "pedal/LowPass.hpp"

class LowPassCombFilter{
  public:
  LowPassCombFilter(float maxDelay = 1000.0f);

  float process(float input);
  float getFeedBackDelayTime();
  float getFeedBackGain();
  float getFeedForwardGain();
  float getMaxDelayTime();
  void setDelayTime(float newFBDelayTime);
  void setDelayByFrequency(float frequency);
  void setFeedBackGain(float newFBGain);
  void setMaxDelayTime(float newMaxDelay);
  
  private:
  float currentSample;
  CircularBuffer delayLine;
  LowPass filter;
  float delayTime;
  float maxDelayTime = 1000.0f;
  float feedBackGain;
};

#endif 