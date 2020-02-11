#include "pedal/Biquad.hpp"
//constructors and deconstructors
//=========================================================
Biquad::Biquad(FilterType initialMode, float initialFrequency, float initialQ){
  frequency = initialFrequency;
  mode = initialMode;
  q = initialQ;
  gain = 0.0f;
  calculateCoefficients();
}
Biquad::~Biquad(){
  delete[] currentBlock;
}
//primary mechanics of class
//========================================================
float* Biquad::processBlock(float* input){
  if(currentBlock != nullptr){//if we don't have a local currentBlock yet, 
    currentBlock = new float[pdlSettings::bufferSize];//create a new array of floats
  }

  for(int i = 0; i < pdlSettings::bufferSize; ++i){//for every sample in the buffer
    //TODO check input block before iterating
    currentBlock[i] = processSample(input[i]);
  }
  return currentBlock;//returns pointer to the begining of this block
}
void Biquad::calculateCoefficients(){
  float norm;
  float v = pow(10, fabs(gain)/20.0f);
  float k = tan(M_PI * frequency);
  //calculate and store these since used frequently
  float k_squared = k * k;
  float kdivbyq = k / q;
  switch(mode){
    case LOW_PASS:
    norm = 1.0f / (1.0f + kdivbyq + k_squared);
    a0 = k_squared * norm;
    a1 = 2.0f * a0;
    a2 = a0;
    b1 = 2.0f * (k_squared - 1.0f) * norm;
    b2 = (1.0f - kdivbyq + k_squared) * norm;
    break;
    case HIGH_PASS:
    norm = 1.0f / (1.0f + kdivbyq + k_squared);
    a0 = 1.0f * norm;
    a1 = -2.0f * a0;
    a2 = a0;
    b1 = 2.0f * (k_squared - 1.0f) * norm;
    b2 = (1.0f - kdivbyq + k_squared) * norm;
    break;
    case BAND_PASS:
    norm = 1.0f / (1.0f + kdivbyq + k_squared);
    a0 = norm;
    a1 = -2.0f * a0;
    a2 = a0;
    b1 = 2.0f * (k_squared - 1.0f) * norm;
    b2 = (1.0f - kdivbyq + k_squared) * norm;
    break;
    case BAND_REJECT:
    norm = 1.0f / (1.0f / kdivbyq + k_squared);
    a0 = (1.0f + k_squared) * norm;
    a1 = 2.0f * (k_squared - 1.0f) * norm;
    a2 = a0;
    b1 = a1;
    b2 = (1.0f - kdivbyq + k_squared) * norm;
    break;
    case PEAK:
    {
    a1 = 2.0f * (k_squared - 1.0f) * norm;
    b1 = a1;
    float vqk = v / q * k;//calculate and store, since used often
    if(gain > 0.0f){
      norm = 1.0f / (1.0f + 1.0f / q * k + k_squared);
      a0 = (1.0f + vqk + k_squared) * norm;
      a2 = (1.0f - vqk + k_squared) * norm;
      b2 = (1.0f - 1.0f / q * k + k_squared) * norm;
    }else{
      norm = 1.0f / (1.0f + vqk + k_squared);
      a0 = (1.0f + 1.0f/q * k + k_squared) * norm;
      a2 = (1.0f - 1.0f/q * k + k_squared) * norm;
      b2 = (1.0f - vqk + k_squared) * norm;
    }
    }
    break;
    case LOW_SHELF:
    {
    float sqrt2k = sqrt(2.0f)*k;//calculate and store, since used often
    float sqrt2vk = sqrt(2.0f * v) * k;
    if(gain >= 0.0f){
      norm = 1.0f / (1.0f + sqrt2k + k_squared);
      a0 = (1.0f + sqrt2vk + v * k_squared)  * norm;
      a1 = 2.0f * (v * k_squared - v) * norm;
      a2 = (1.0f - sqrt2vk + v * k_squared) * norm;
      b1 = 2.0f * (k_squared - 1.0f) * norm;
      b2 = (1.0f - sqrt2k + k_squared) * norm;
    }else{
      norm = 1.0f / (1.0f + sqrt2vk + v * k_squared);
      a0 = (1.0f + sqrt2k + k_squared) * norm;
      a1 = 2.0f * (k_squared - 1.0f) * norm;
      a2 = (1.0f - sqrt2k + k_squared) * norm;
      b1 = 2.0f * (v * k_squared - 1.0f) * norm;
      b2 = (1.0f - sqrt2vk + k_squared) * norm;
    }
    }
    break;
    case HIGH_SHELF:
    {
    float sqrt2k = sqrt(2.0f)*k;//calculate and store, since used often
    float sqrt2vk = sqrt(2.0f * v) * k;
    if(gain >= 0.0f){
      norm = 1.0f / (1.0f + sqrt2k + k_squared);
      a0 = (v + sqrt2vk + k_squared) * norm;
      a1 = 2.0f * (k_squared - v) * norm;
      a2 = (v - sqrt2vk + k_squared) * norm;
      b1 = 2.0f * (k_squared - 1.0f) * norm;
      b2 = (1.0f - sqrt2k + k_squared) * norm;
    }else{
      norm = 1.0f / (v + sqrt2vk + k_squared);
      a0 = (1.0f + sqrt2vk + k_squared) * norm;
      a1 = 2.0f * (k_squared - 1.0f) * norm;
      a2 = (1.0f - sqrt2k + k_squared) * norm;
      b1 = 2.0f * (k_squared - v) * norm;
      b2 = (v - sqrt2vk + k_squared) * norm;
    }
    }
    break; 
  }
}
void Biquad::flush(){
  z1 = z2 = 0.0f;
}
//Getters and setters
//=========================================================
void Biquad::setFrequency(float newFrequency){
  frequency = newFrequency/pdlSettings::sampleRate;
  calculateCoefficients();
}
void Biquad::setGain(float newGain){
  gain = newGain;
  calculateCoefficients();
}
void Biquad::setQ(float newQ){
  q = newQ;
  calculateCoefficients();
}
void Biquad::setMode(FilterType newMode){
  mode = newMode;
  flush();
  calculateCoefficients();
}
//frequency is stored in normalized form, so must adjust to retrieve
float Biquad::getFrequency(){return frequency * pdlSettings::sampleRate;}
float Biquad::getGain(){return gain;}
float Biquad::getQ(){return q;}
FilterType Biquad::getMode(){return mode;}
float Biquad::getSample(){return currentSample;}
float* Biquad::getBlock(){return currentBlock;}