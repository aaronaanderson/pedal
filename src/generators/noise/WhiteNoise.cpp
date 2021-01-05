#include "pedal/WhiteNoise.hpp"

using namespace pedal;

//constructors and deconstructors
//=========================================================
WhiteNoise::WhiteNoise(){
  currentSample = generateSample();//start wtih a random sample
}
WhiteNoise::~WhiteNoise(){}//when object is deleted

//primary mechanics of class
//=========================================================
float WhiteNoise::generateSample(){//generate a single sample
  currentSample = rangedRandom(-1.0f, 1.0f);//very basic random....could be more random
  return currentSample;
}
//Getters and setters
//=========================================================
float WhiteNoise::getSample(){return currentSample;}
