#include "pedal/WhiteNoise.hpp"

WhiteNoise::WhiteNoise(){
  currentSample = process();
}

float WhiteNoise::process(){
  currentSample = rangedRandom(-1.0f, 1.0f);
}

float WhiteNoise::getSample(){return currentSample;}

