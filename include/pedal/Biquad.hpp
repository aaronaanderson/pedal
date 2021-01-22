#ifndef Biquad_hpp
#define Biquad_hpp

#include "Constants.hpp"
#include <cmath>
#include "Settings.hpp"

/*
This biquad object was based heavily on Nigel Redmon's
implementation:
https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
*/
namespace pedal{
class Biquad{
  public:
  enum class Mode{
  LOW_PASS,
  HIGH_PASS,
  BAND_PASS,
  BAND_REJECT,
  PEAK,
  LOW_SHELF,
  HIGH_SHELF
  };
  Biquad(Mode initialMode = Mode::LOW_SHELF, 
         float initialFrequency = 2000.0f/Settings::sampleRate,
         float initialQ = 0.7f);
  ~Biquad();

  inline float processSample(float input);
  float* processBlock(float* input);
  void clearHistory();//0.0f history
  
  void setBiquad(Mode mode, float newFrequency, 
                 float newQ, float newGain);
  void setFrequency(float newFrequency);
  void setGain(float newGain);
  void setQ(float newQ);
  void setMode(Mode newMode);

  float getFrequency();
  float getGain();
  float getQ();
  Mode getMode();
  float getSample();
  float* getBlock();

  private:
  void calculateCoefficients();//called when parameters change
  float frequency;//center/cutoff/shelf frequency depending on mode
  //Frequency is stored in normalized mode to reduce calculations (0.0 -> 1.0)
  double q;
  double gain;
  Mode mode;//enumerated above
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
}//end pedal namespace
#endif