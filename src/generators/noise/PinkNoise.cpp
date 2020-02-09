#include "pedal/PinkNoise.hpp"

#include "math.h"
PinkNoise::PinkNoise(){
  indexMax = pow(2, NUM_OCTAVES) - 1;
  runningSum = 0.0f;
  //generate a random value to start for all white noise generators
  for(int i = 0; i < NUM_OCTAVES; i++){
    runningSum += noiseArray[i].generateSample();
  }
  index = 0;//where are we in the 'whichOctave' logic?
}

float PinkNoise::generateSample(){
  int whichOctave = getOctave(index);
  index++;
  if(index >= indexMax){index = 0;}
  //before adding the new noise value, we must subtract the previous
  runningSum -= noiseArray[whichOctave].getSample();
  //now we replace it with the new sample
  runningSum += noiseArray[whichOctave].generateSample();
  currentSample = runningSum + rangedRandom(-1.0f, 1.0f);
  return currentSample;
}

void PinkNoise::checkGetOctaveLogic(){
  unsigned numberOfSteps = pow(2, NUM_OCTAVES)-1;
  for(int i = 0; i < numberOfSteps; i++){
    std::cout << "There are " <<  NUM_OCTAVES << 
                 "octaves, requiring " << numberOfSteps << 
                 "steps" << std::endl;
    int whichOctave = PinkNoise::getOctave(i) + 1;
    std::cout << whichOctave << "|";
    for(int j = 0; j < whichOctave; j++){
      std::cout << "*" ;
    }
    std::cout << "\n";
  }
}