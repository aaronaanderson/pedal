#ifndef Biquad_hpp
#define Biquad_hpp

#define _USE_MATH_DEFINES
#include <cmath>
#include "pdlSettings.hpp"

/*
This biquad object was based heavily on Nigel Redmon's
implementation:
https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
*/

class Biquad{
  public:
  enum class modes{
  LOW_PASS,
  HIGH_PASS,
  BAND_PASS,
  BAND_REJECT,
  PEAK,
  LOW_SHELF,
  HIGH_SHELF
  };
  Biquad(modes initialMode = modes::LOW_SHELF, 
         float initialFrequency = 2000.0f/pdlSettings::sampleRate,
         float initialQ = 0.7f);
  ~Biquad();

  inline float processSample(float input);
  float* processBlock(float* input);
  void flush();//0.0f history
  
  void setBiquad(modes mode, float newFrequency, 
                 float newQ, float newGain);
  void setFrequency(float newFrequency);
  void setGain(float newGain);
  void setQ(float newQ);
  void setMode(modes newMode);

  float getFrequency();
  float getGain();
  float getQ();
  modes getMode();
  float getSample();
  float* getBlock();

  private:
  void calculateCoefficients();//called when parameters change
  float frequency;//center/cutoff/shelf frequency depending on mode
  //Frequency is stored in normalized mode to reduce calculations (0.0 -> 1.0)
  double q;
  double gain;
  modes mode;//enumerated above
  double a0, a1, a2;//feed-forward coefficients
  double b1, b2;//feed-back coefficients
  double z1, z2;//containers for previous output
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