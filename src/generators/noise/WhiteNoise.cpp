#include "pedal/WhiteNoise.hpp"

WhiteNoise::WhiteNoise(){
  currentSample = generateSample();
}

float WhiteNoise::generateSample(){
  currentSample = rangedRandom(-1.0f, 1.0f);
}

float WhiteNoise::getSample(){return currentSample;}