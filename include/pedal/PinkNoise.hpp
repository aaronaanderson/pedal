#ifndef PinkNoise_hpp
#define PinkNoise_hpp

#include <iostream>
#include <vector>
#include "WhiteNoise.hpp"

//0xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx 1  | 0
//1x x x x x x x x x x x x x x x x 2  | 0
//2 x   x   x   x   x   x   x   x  4  | 1
//3   x       x       x       x    8  | 3
//4       x               x        16 | 7 
//5               x                32 | 15



class PinkNoise{
  public:
  PinkNoise(int octaves = 12);
  int getOctave(int index){
    int indexLocation = index;
    int whichOctave = 0;
    while(indexLocation % 2 == 1){
      whichOctave++;
      indexLocation /= 2;
    }
    return whichOctave;
  }
  private:
  std::vector<WhiteNoise> noiseArray;
  unsigned numberOctaves;

};
#endif