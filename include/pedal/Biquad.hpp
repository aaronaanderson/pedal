#ifndef Biquad_hpp
#define Biquad_hpp

//in progress....


enum FilterType{
  LOW_PASS = 0,
  HIGH_PASS,
  BAND_PASS,
  RESONANT,
  BAND_REJECT,
  ALL_PASS,
  PEAKING,
  LOW_SHELF,
  HIGH_SHELF
};

class Biquad{
  public:
  Biquad();
  ~Biquad();

  inline float processSample(float input);
  float* processBlock(float* input);

  void setFrequency(float newFrequency);
  void setResonance(float newResonance);
  void setLevel(float newLevel);
  void setMode(FilterType newMode);

  float getFrequency();
  float getResonance();
  float getLevel();
  FilterType getMode();

  float getSample();
  float* getBlock();

  private:
  void calculateCoefficients();
  float frequency;//center/cutoff/shelf frequency depending on mode
  FilterType mode;//enumerated above
  float a0, a1, a2;//feed-forward coefficients
  float b1, b2;//feed-back coefficients
  float z1, z2;//containers for previous output
  float currentSample;//this could be considered z0 in this case
  float* currentBlock;//storage for processing entire block
};
#endif