#ifndef Gate_hpp
#define Gate_hpp

#include "pedal/CircularBuffer.hpp"
#include "pedal/BufferedRMS.hpp"
#include "pedal/utilities.hpp"
/*
A gate makes quiet sounds more quiet. How much more
quiet depends on how high the ratio is, and how quiet
the input was to begin with. 
A gate is a special case of an expander; it is a downward 
expander with a high ratio. Typically these are used to 
reduce microphone bleed or reduce noise from a signal. 
*/
class Gate{
  public:
  Gate();
  float process(float input);
  float process(float input, float sideChain);

  void setThresholdDB(float newThresholdDB);//signal below threshold triggers attenuation
  void setRatio(float newRatio);//set attenuation ratio
  void setRangeDB(float newRangeDB);//if below signal, reduce by this amount in addition
  void setInputGainDB(float inputGainDB);//set input gain in dB
  void setInputGainLinear(float inputGainLinear);//set by scalar
  void setMakeUpGainDB(float newMakeUpGainDB);//amount to scale output by
  void setAttackTime(float newAttackTime);//set attack time (ms)
  void setHoldTime(float newHoldTime);//how long to wait after attack and before release
  void setReleaseTime(float newDecayTime);//set release time (ms)
  void setLookAheadTime(float newLookAhead);//set look-ahead time in ms
  void setAnalysisTime(float newAnalysisTime);//16 sample default
  
  float getSample();
  float getLinearScalar();//get final smoothed scalar value
  float getThresholdDB();
  float getRatio();//
  float getRangeDB();//immediate dropoff amount before slope
  float getInputGainDB();//get inputGain in dB
  float getMakeUpGainDB();//get makeUpGain in dB
  float getAttackTime();//(ms)
  float getReleaseTime();//(ms)
  float getLookAheadTime();//(ms)
  float getAnalysisTime();//(ms)
  
  private:
  void updateGain(float input);
  float currentSample;//store current working sample
  CircularBuffer delayLine;//analysis takes time, compensate output
  BufferedRMS signalEstimator;//somewhat expensive amplitude follower
  float linearThreshold;//store a copy as a scalar
  float threshold;//stored in dB
  float ratio;//how many dB of reduction per dB over threshold
  float rangeDB;//how many dB to drop before slope
  float linearInputGain;//what value to multiply input by for signal estimation
  bool attackFlag;// keep track of state (attack or release)
  DirectionalSmoothValue<float> linearGain;//special SmoothValue that has different arrival times for up or down
  int attackPhaseInSamples;//current location in attack phase
  int attackTimeInSamples;//cheaper to store int location
  int holdTimeInSamples;//how many samples should the hold phase last
  float lowestAttackPhaseTarget;//keep a running minimum for attack/hold phase
  float analysisTime;//how long does signalEstimator take?
  float lookAhead;//push analysis further ahead of 
  float linearMakeUpGain;//how much to scale the output
  float reductionTargetDB;//how much reduction is being attempted
};
#endif