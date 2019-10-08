#ifndef HanningWindow_hpp
#define HanningWindow_hpp

#include "pdlSettings.hpp"

class HanningWindow{
  public:
  HanningWindow();
  ~HanningWindow();

  float generateSample();
  float* generateBlock();
  static float sampleFromPhase(float newPhase);//expecting 0-1
  
  void setDuration(float newDuration);

  float getCurrentSample();
  float* getCurrentBlock();
  float getDuration();

  private:
  
  float duration;
  double phaseIncrement;
  float currentSample;
  float* currentBlock;
};
#endif