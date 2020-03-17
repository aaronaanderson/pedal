#ifndef Compressor_hpp
#define Compressor_hpp

#include "pedal/CircularBuffer.hpp"
#include "pedal/BufferedRMS.hpp"
#include "pedal/utilities.hpp"
/*
This is a basic compressor. More specifically, this 
is a dynamic compressor; it comresses the dynamic range
of an incomming signal. If a signal is louder than a 
set threshold, it is attenuated by an amount derived from
the amount in dB the threshold was exceeded, and the 
ratio. 
*/
class Compressor{
  public:
  Compressor();//default constructor

  float process(float input);//main per-sample function
  float process(float input, float sideChain);//overload; if second input is given use side-chain mode
  
  void setThresholdDB(float newThresholdDB);//At what intensity does attenuation begin?
  void setRatio(float newRatio);//set attenuation ratio
  void setInputGainDB(float inputGainDB);//set input gain in dB
  void setInputGainLinear(float inputGainLinear);//set by scalar
  void setMakeUpGainDB(float newMakeUpGainDB);
  void setAttackTime(float newAttackTime);//set attack time (ms)
  void setReleaseTime(float newDecayTime);//set release time (ms)
  void setLookAheadTime(float newLookAhead);//set look-ahead time in ms
  void setAnalysisTime(float newAnalysisTime);//16 sample default
  
  float getSample();
  float getLinearScalar();//get final smoothed scalar value
  float getThresholdDB();
  float getRatio();
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
  float dBDifferenceScalar;//derived from ratio
  float linearInputGain;//what value to multiply input by for signal estimation
  bool attackFlag;// keep track of state (attack or release)
  DirectionalSmoothValue<float> linearGain;//special SmoothValue that has different arrival times for up or down
  int attackPhaseInSamples;//current location in attack phase
  int attackTimeInSamples;//cheaper to store int location
  float highestAttackPhaseTarget;
  float analysisTime;//how long does signalEstimator take?
  float lookAhead;//push analysis further ahead of 
  float linearMakeUpGain;//how much to scale the output
  float reductionTargetDB;//how much reduction is being attempted
};
#endif 

//On Ratios
/*
The ratio determines how much to scale the output if the
analyzed signal exceeds the threshold. The amount of reduction
depends on both how much a signal exceeds the threshold and on
the value set for ratio. If ratio is 4 and the inpu exceeds the 
threshold by 8dB, the output signal will be reduced to 
8/4dB, or 2dB above the threshold
*/
//On side-chaining
/*
Side chaining means to use different signals for estimation and
compression. In any case, the input of the process is the signal
that will be compressed. If no side-chain is provided, the input
is used in the estimator. If a side-chain is provided, the 
side-chain will be used in the estimator to determine how much to 
compress the input. A common commercial music technique is to compress
a bass guitar with a kick drum as a side-chain input. The bass guitar
will be attenuated if the kick drum is loud enough. This works well 
as the two instruments share a lot of frequency components and have to
fight eachother for space in a mix. 
*/
