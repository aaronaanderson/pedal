#include "pedal/LowFrequencyOscillator.hpp"

using namespace pedal;

#include <iostream>

LowFrequencyOscillator::LowFrequencyOscillator(){
  frequency = 4.0f;
  amplitude = 1.0f;
  phase = 0.0;
  setOutputRange(-1.0f, 1.0f);
  currentWaveShape = WaveShape::Sine;
  setWaveShape(WaveShape::Sine);
}
LowFrequencyOscillator::~LowFrequencyOscillator(){
  deletePreviousOscillator(currentWaveShape);
}
float LowFrequencyOscillator::generateSample(){
  switch(currentWaveShape){
    case WaveShape::Sine:
      currentSample = std::get<(int)WaveShape::Sine>(oscillator)->generateSample();
    break;
    case WaveShape::Triangle:
      currentSample = std::get<(int)WaveShape::Triangle>(oscillator)->generateSample();
    break;
    case WaveShape::Square:
      currentSample = std::get<(int)WaveShape::Square>(oscillator)->generateSample();
    break;
    case WaveShape::Saw:
      currentSample = std::get<(int)WaveShape::Saw>(oscillator)->generateSample();
    break;
    default:
      currentSample = 0.0f;
    break;
  }
  if(shouldFlipWaveform){currentSample = -1.0f;}
  currentSample *= amplitude;
  currentSample = map(currentSample, -1.0f, 1.0f, outputRangeLowerBound, outputRangeUpperBound);
  return currentSample;
}

void LowFrequencyOscillator::setFrequency(float newFrequency){
  frequency = newFrequency;
  switch(currentWaveShape){
    case WaveShape::Sine:
      std::get<(int)WaveShape::Sine>(oscillator)->setFrequency(newFrequency);
    break;
    case WaveShape::Triangle:
      std::get<(int)WaveShape::Triangle>(oscillator)->setFrequency(newFrequency);
    break;
    case WaveShape::Square:
      std::get<(int)WaveShape::Square>(oscillator)->setFrequency(newFrequency);
    break;
    case WaveShape::Saw:
      std::get<(int)WaveShape::Saw>(oscillator)->setFrequency(newFrequency);
    break;
  }
}
void LowFrequencyOscillator::setPhase(float newPhase){
  phase = newPhase;
  switch(currentWaveShape){
    case WaveShape::Sine:
      std::get<(int)WaveShape::Sine>(oscillator)->setPhase(newPhase);
    break;
    case WaveShape::Triangle:
      std::get<(int)WaveShape::Triangle>(oscillator)->setPhase(newPhase);
    break;
    case WaveShape::Square:
      std::get<(int)WaveShape::Square>(oscillator)->setPhase(newPhase);
    break;
    case WaveShape::Saw:
      std::get<(int)WaveShape::Saw>(oscillator)->setPhase(newPhase);
    break;
  }
}
void LowFrequencyOscillator::setAmplitude(float newAmplitude){amplitude = newAmplitude;}
void LowFrequencyOscillator::flipWaveform(bool newShouldFlipWaveform){shouldFlipWaveform = newShouldFlipWaveform;}
void LowFrequencyOscillator::setWaveShape(WaveShape newWaveShape){
  deletePreviousOscillator(currentWaveShape);
  currentWaveShape = newWaveShape;
  switch(currentWaveShape){
    case WaveShape::Sine:
      oscillator = new TrivialSine(frequency);
    break;
    case WaveShape::Triangle:
      oscillator = new TrivialTriangle(frequency);
    break;
    case WaveShape::Square:
      oscillator = new TrivialSquare(frequency);
    break;
    case WaveShape::Saw:
      oscillator = new TrivialSaw(frequency);
    break;
  }
}
void LowFrequencyOscillator::setOutputRange(float newOutputMin, float newOutputMax){
    outputRangeLowerBound = newOutputMin;
    outputRangeUpperBound = newOutputMax;
}
float  LowFrequencyOscillator::getSample(){return currentSample;}
float  LowFrequencyOscillator::getFrequency(){return frequency;}
double LowFrequencyOscillator::getPhase(){return phase;}
float  LowFrequencyOscillator::getAmplitude(){return amplitude;}
bool   LowFrequencyOscillator::getShouldFlipWaveform(){return shouldFlipWaveform;}
LowFrequencyOscillator::WaveShape LowFrequencyOscillator::getWaveShape(){return currentWaveShape;}
float LowFrequencyOscillator::getOutputRangeLowerBound(){return outputRangeLowerBound;}
float LowFrequencyOscillator::getOutputRangeUpperBound(){return outputRangeUpperBound;}

void LowFrequencyOscillator::deletePreviousOscillator(WaveShape previousShape){
  switch(previousShape){
    case WaveShape::Sine:
      if(auto ptr = std::get<TrivialSine*>(oscillator))
        delete ptr;
    break;
    case WaveShape::Triangle:
      if(auto ptr = std::get<TrivialTriangle*>(oscillator))
        delete ptr;
    break;
    case WaveShape::Square:
      if(auto ptr = std::get<TrivialSquare*>(oscillator))
        delete ptr;
    break;
    case WaveShape::Saw:
      if(auto ptr = std::get<TrivialSaw*>(oscillator))
        delete ptr;
    break;
  }
}