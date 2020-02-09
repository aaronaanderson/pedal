#ifndef PinkNoise_hpp
#define PinkNoise_hpp

#include <iostream>
#include <vector>
#include "WhiteNoise.hpp"

#define NUM_OCTAVES 12
/*
//0xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 1  | 0
//1x x x x x x x x x x x x x x x x 2  | 0
//2 x   x   x   x   x   x   x   x  4  | 1
//3   x       x       x       x    8  | 3
//4       x               x        16 | 7 
//5               x                32 | 15
*/
class PinkNoise{
  public:
  PinkNoise(int octaves = 12);
  float generateSample();
  
  private:
  WhiteNoise noiseArray[NUM_OCTAVES];
  unsigned index;//
  unsigned indexMax;
  float runningSum;
  float currentSample;
  void checkGetOctaveLogic();
  inline int getOctave(int index){
    int indexLocation = index;
    int whichOctave = 0;
    while(indexLocation % 2 == 1){
      whichOctave++;
      indexLocation /= 2;
    }
    return whichOctave;
  }
};
#endif