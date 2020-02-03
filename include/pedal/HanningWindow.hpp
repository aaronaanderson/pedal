#ifndef HanningWindow_hpp
#define HanningWindow_hpp

#include "pdlSettings.hpp"
#include "math.h"

class HanningWindow{
  public:
  HanningWindow();
  HanningWindow(float initialDuration);
  ~HanningWindow();

  float generateSample();
  float* generateBlock();
  inline float sampleFromPhase(float inputPhase){
    return 0.5f * (1.0f - cos((2.0f * 3.1415926 * inputPhase)/1.0f));
  }
  void setTrigger(bool newTrigger);
  void setDuration(float newDuration);
  void setPhase(float newPhase);

  float getCurrentSample();
  float* getCurrentBlock();
  float getDuration();
  float getPhase();

  private:
  bool trigger;
  bool active;
  float duration;
  float phase, phaseIncrement;
  float currentSample;
  float* currentBlock = nullptr;
};
#endif