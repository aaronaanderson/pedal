#ifndef Biquad_hpp
#define Biquad_hpp

#include "math.h"
#include "pdlSettings.hpp"
//in progress....

enum FilterType{
  LOW_PASS = 0,
  HIGH_PASS,
  BAND_PASS,
  BAND_REJECT,
  PEAK,
  LOW_SHELF,
  HIGH_SHELF
};

class Biquad{
  public:
  Biquad(FilterType initialMode = LOW_PASS, 
         float initialFrequency = 2000.0f,
         float initialQ = 2.0f);
  ~Biquad();

  inline float processSample(float input);
  float* processBlock(float* input);
  void flush();//0.0f history
  
  void setFrequency(float newFrequency);
  void setGain(float newGain);
  void setQ(float newQ);
  void setMode(FilterType newMode);

  float getFrequency();
  float getGain();
  float getQ();
  FilterType getMode();
  float getSample();
  float* getBlock();

  private:
  void calculateCoefficients();//called when parameters change
  float frequency;//center/cutoff/shelf frequency depending on mode
  //Frequency is stored in normalized mode to reduce calculations (0.0 -> 1.0)
  float q;
  float gain;
  FilterType mode;//enumerated above
  float a0, a1, a2;//feed-forward coefficients
  float b1, b2;//feed-back coefficients
  float z1, z2;//containers for previous output
  float currentSample;
  float* currentBlock = nullptr;//storage for processing entire block
};
/*
Transposed Direct Form II, thanks to Nigel Redmon
https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
*/
inline float Biquad::processSample(float input){
  currentSample = input * a0 + z1;
  z1 = input * a1 + z2 - b1 * currentSample;
  z2 = input * a2 - b2 * currentSample;
  return currentSample;
}
#endif