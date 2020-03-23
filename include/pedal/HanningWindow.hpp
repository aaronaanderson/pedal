#ifndef HanningWindow_hpp
#define HanningWindow_hpp

#include "pdlSettings.hpp"
#define _USE_MATH_DEFINES
#include "utilities.hpp"
#include <cmath>

class HanningWindow{
  public:
  HanningWindow();
  HanningWindow(float initialDuration);

  float generateSample();
  static inline float sampleFromPhase(float inputPhase){
    return 0.5f * (1.0f - std::cos(2.0f * M_PI * inputPhase));
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
};
#endif