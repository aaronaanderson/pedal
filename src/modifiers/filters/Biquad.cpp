#include "pedal/Biquad.hpp"

using namespace pedal;
/*
This biquad object was based on Nigel Redmon's
implementation:
https://www.earlevel.com/main/2012/11/26/biquad-c-source-code/
*/
//constructors and deconstructors
//=========================================================
Biquad::Biquad(Mode initialMode, float initialFrequency, float initialQ){
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
  double norm;
  double v = std::pow(10, fabs(gain)/20.0);
  double k = std::tan(pedal::PI * frequency);
  //calculate and store these since used frequently
  double kSquared = k * k;
  double kDivByQ = k / q;
  switch(mode){
    case Mode::LOW_PASS:
      norm = 1.0 / (1.0 + kDivByQ + kSquared);
      a0 = kSquared * norm;
      a1 = 2.0 * a0;
      a2 = a0;
      b1 = 2.0 * (kSquared - 1.0) * norm;
      b2 = (1.0 - kDivByQ + kSquared) * norm;
    break;
    case Mode::HIGH_PASS:
      norm = 1.0 / (1.0 + kDivByQ + kSquared);
      a0 = 1.0 * norm;
      a1 = -2.0 * a0;
      a2 = a0;
      b1 = 2.0 * (kSquared - 1.0) * norm;
      b2 = (1.0 - kDivByQ + kSquared) * norm;
    break;
    case Mode::BAND_PASS:
      norm = 1.0 / (1.0 + kDivByQ + kSquared);
      a0 = k / q * norm;
      a1 = 0.0;
      a2 = -a0;
      b1 = 2.0 * (kSquared - 1.0) * norm;
      b2 = (1.0 - kDivByQ + kSquared) * norm;
    break;
    case Mode::BAND_REJECT:
      norm = 1.0 / (1.0 + kDivByQ + kSquared);
      a0 = (1.0 + kSquared) * norm;
      a1 = 2.0 * (kSquared - 1.0) * norm;
      a2 = a0;
      b1 = a1;
      b2 = (1.0 - kDivByQ + kSquared) * norm;
    break;
    case Mode::PEAK:
    {
      a1 = 2.0 * (kSquared - 1.0) * norm;
      b1 = a1;
      double vqk = v / q * k;//calculate and store, since used often
      if(gain >= 0.0f){
        norm = 1.0 / (1.0 + 1.0 / q * k + kSquared);
        a0 = (1.0 + vqk + kSquared) * norm;
        a2 = (1.0 - vqk + kSquared) * norm;
        b2 = (1.0 - 1.0 / q * k + kSquared) * norm;
      }else{
        norm = 1.0 / (1.0 + vqk + kSquared);
        a0 = (1.0 + 1.0 / q * k + kSquared) * norm;
        a2 = (1.0 - 1.0 / q * k + kSquared) * norm;
        b2 = (1.0 - v / q * k + kSquared) * norm;
      }
    }
    break;
    case Mode::LOW_SHELF:
    {
      double sqrt2k = std::sqrt(2.0f)*k;//calculate and store, since used often
      double sqrt2vk = std::sqrt(2.0f * v) * k;
      if(gain >= 0.0){
        norm = 1.0 / (1.0 + sqrt2k + kSquared);
        a0 = (1.0 + sqrt2vk + v * kSquared)  * norm;
        a1 = 2.0 * (v * kSquared - 1.0) * norm;
        a2 = (1.0 - sqrt2vk + v * kSquared) * norm;
        b1 = 2.0 * (kSquared - 1.0) * norm;
        b2 = (1.0 - sqrt2k + kSquared) * norm;
      }else{
        norm = 1.0 / (1.0 + sqrt2vk + v * kSquared);
        a0 = (1.0 + sqrt2k + kSquared) * norm;
        a1 = 2.0 * (kSquared - 1.0) * norm;
        a2 = (1.0 - sqrt2k + kSquared) * norm;
        b1 = 2.0 * (v * kSquared - 1.0) * norm;
        b2 = (1.0 - sqrt2vk + v * kSquared) * norm;
      }
    }
    break;
    case Mode::HIGH_SHELF:
    {
      float sqrt2k = std::sqrt(2.0)*k;//calculate and store, since used often
      float sqrt2vk = std::sqrt(2.0 * v) * k;
      if(gain >= 0.0f){
        norm = 1.0 / (1.0 + sqrt2k + kSquared);
        a0 = (v + sqrt2vk + kSquared) * norm;
        a1 = 2.0 * (kSquared - v) * norm;
        a2 = (v - sqrt2vk + kSquared) * norm;
        b1 = 2.0 * (kSquared - 1.0) * norm;
        b2 = (1.0 - sqrt2k + kSquared) * norm;
      }else{
        norm = 1.0 / (v + sqrt2vk + kSquared);
        a0 = (1.0 + sqrt2vk + kSquared) * norm;
        a1 = 2.0 * (kSquared - 1.0) * norm;
        a2 = (1.0 - sqrt2k + kSquared) * norm;
        b1 = 2.0 * (kSquared - v) * norm;
        b2 = (v - sqrt2vk + kSquared) * norm;
      }
    }
    break; 
  }
}
void Biquad::clearHistory(){
  z1 = z2 = 0.0;
}
//Getters and setters
//=========================================================
void Biquad::setBiquad(Mode newMode, float newFrequency, 
                       float newQ, float newGain){
  mode = newMode;
  frequency = newFrequency/pdlSettings::sampleRate;
  q = newQ;
  gain = newGain;
  calculateCoefficients();
}
void Biquad::setFrequency(float newFrequency){
  if(newFrequency != frequency){
    frequency = newFrequency/pdlSettings::sampleRate;
    calculateCoefficients();
  }
}
void Biquad::setGain(float newGain){
  if(newGain != gain){
    gain = newGain;
    calculateCoefficients();
  }
}
void Biquad::setQ(float newQ){
  if(newQ != q){
    q = newQ;
    calculateCoefficients();
  }
}
void Biquad::setMode(Mode newMode){
  if(newMode != mode){
    mode = newMode;
    clearHistory();
    calculateCoefficients();
  }
}
//frequency is stored in normalized form, so must adjust to retrieve
float Biquad::getFrequency(){return frequency * pdlSettings::sampleRate;}
float Biquad::getGain(){return gain;}
float Biquad::getQ(){return q;}
Biquad::Mode Biquad::getMode(){return mode;}
float Biquad::getSample(){return currentSample;}
float* Biquad::getBlock(){return currentBlock;}