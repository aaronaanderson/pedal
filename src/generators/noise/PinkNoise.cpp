#include "pedal/PinkNoise.hpp"

#include "math.h"
PinkNoise::PinkNoise(int octaves){
  int steps = pow(2, 12);
  for(int i = 0; i < steps; i++){
    //std::cout << PinkNoise::getOctave(i) << std::endl;
    int whichOctave = PinkNoise::getOctave(i) + 1;
    std::cout << whichOctave << "|";
    for(int j = 0; j < whichOctave; j++){
      std::cout << "*" ;
    }
    std::cout << "\n";
  }
}
';
"?
\'
