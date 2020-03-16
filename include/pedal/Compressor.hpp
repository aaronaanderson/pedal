#ifndef Compressor_hpp
#define Compressor_hpp

#include "pedal/CircularBuffer.hpp"
#include "pedal/BufferedRMS.hpp"
#include "pedal/utilities.hpp"
#include "pedal/DebugTool.hpp"
/*

*/
class Compressor{
  public:
  Compressor();//default constructor

  float process(float input);//main per-sample function
  
  void setThresholdDB(float newThresholdDB);//At what intensity does attenuation begin?
  void setRatio(float newRatio);//set attenuation ratio
  void setInputGainDB(float inputGainDB);//set input gain in dB
  void setInputGainLinear(float inputGainLinear);//set by scalar
  void setAttackTime(float newAttackTime);//set attack time (ms)
  void setReleaseTime(float newDecayTime);//set release time (ms)
  void setLookAheadTime(float newLookAhead);//set look-ahead time in ms
  void setAnalysisTime(float newAnalysisTime);//16 sample default
  
  float getSample();
  float getThresholdDB();
  float getRatio();
  float getInputGainDB();//get inputGain in dB
  float getAttackTime();//(ms)
  float getReleaseTime();//(ms)
  float getLookAheadTime();//(ms)
  float getAnalysisTime();//(ms)

  private:
  float currentSample;//store current working sample
  CircularBuffer delayLine;//analysis takes time, compensate output
  BufferedRMS signalEstimator;//somewhat expensive amplitude follower
  float threshold;//stored as a SCALAR, not log
  float ratio;//how many dB of reduction per dB over threshold
  float linearInputGain;//what value to multiply input by for signal estimation
  bool attackFlag;// keep track of state (attack or release)
  DirectionalSmoothValue<float> linearGain;//special SmoothValue that has different arrival times for up or down
  int attackPhaseInSamples;//current location in attack phase
  int attackTimeInSamples;//cheaper to store int location
  float analysisTime;//how long does signalEstimator take?
  float lookAhead;//push analysis further ahead of 
  DebugTool debug;
};
#endif 