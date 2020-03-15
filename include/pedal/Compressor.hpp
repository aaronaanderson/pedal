#ifndef Compressor_hpp
#define Compressor_hpp

#include "pedal/CircularBuffer.hpp"
#include "pedal/BufferedRMS.hpp"
#include "pedal/utilities.hpp"

class Compressor{
  public:
  Compressor();

  float process(float input);

  void setRatio(float newRatio);
  void setAttackTime(float newAttackTime);
  void setReleaseTime(float newDecayTime);
  void setThreshold(float newThresholdDB);
  void setAnalysisTime(float newAnalysisTime);

  float getSample();
  float getRatio();
  float getAttackTime();
  float getReleaseTime();
  float getThreshold();
  float getAnalysisTime();

  private:
  float currentSample;//store current working sample
  CircularBuffer delayLine;//analysis takes time, compensate output
  BufferedRMS signalEstimator;//somewhat expensive amplitude follower
  float ratio;//how many dB of reduction per dB over threshold
  bool attackFlag;// keep track of state (attack or release)
  DirectionalSmoothValue linearGain;//special SmoothValue that has different arrival times for up or down
  int attackPhaseInSamples;//current location in attack phase
  int attackTimeInSamples;//cheaper to store int location
  float currentInverseRatio;//active final scalar 
  float attackCoefficient;//dependent on sample rate and attack time
  float releaseCoefficient;//dependent on sample rate and attack time
  float threshold;//stored as a SCALAR, not log
  float analysisTime;//how long does signalEstimator take?
}
#endif 