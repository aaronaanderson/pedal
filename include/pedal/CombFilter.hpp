#ifndef CombFilter_hpp
#define CombFilter_hpp

#include "CircularBuffer.hpp"//used for delay line

/* Filter Diagram

         -->[DELAY1]*a   b*[DELAY2]<--
        /             \ /             \
input__/_______________+_______________\__output
a = feedForwardGain, b = feedBackGain
DELAY1 = inputDelayBuffer
DELAY2 = outputDelayBuffer
*/
class CombFilter{
  public:
  CombFilter();
  
  float filter(float input);
  
  float getFeedForwardDelayTime();
  float getFeedBackDelayTime();
  float getFeedBackGain();
  float getFeedForwardGain();
  float getMaxDelayTime();
  void setFeedForwardDelayTime(float newFFDelayTime);
  void setFeedBackDelayTime(float newFBDelayTime);
  void setFFDelayByFrequency(float frequency, bool peak = true);
  void setFBDelayByFrequency(float frequency, bool peak = true);
  void setFeedBackGain(float newFBGain);
  void setFeedForwardGain(float newFFGain);
  void setMaxDelayTime(float newMaxDelay);
  private:
  float currentSample;
  CircularBuffer inputDelayBuffer;
  CircularBuffer outputDelayBuffer;
  float feedForwardDelayTime;
  float feedBackDelayTime;
  float delayTime;
  float maxDelayTime = 1000.0f;
  float feedForwardGain;
  float feedBackGain;
};
#endif 