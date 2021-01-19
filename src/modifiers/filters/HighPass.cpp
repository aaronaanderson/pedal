#include "pedal/HighPass.hpp"

using namespace pedal;

HighPass::HighPass(float frequency){
  setFrequency(frequency);
  currentSample = 0.0f;
}
//float LowPass::process(float input) is in the header
//because it is inlined
void HighPass::setFrequency(float newFrequency){
  //These coefficient calculations are from Will Pirkle's
  //Designing Audio Effect Plug-ins in C++, pg 165
  float theta = (pedal::TWOPI * newFrequency)/Settings::sampleRate;
  float gamma = 2.0f + std::cos(theta);
  b = gamma - std::sqrt((gamma * gamma) - 1.0f);
  a = 1.0f - b;
}
float HighPass::getSample(){return currentSample;}