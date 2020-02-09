#include "pedal/PinkNoise.hpp"
//constructors and deconstructors
//=========================================================
PinkNoise::PinkNoise(){
  indexMax = pow(2, NUM_OCTAVES) - 1;//size of the octave pattern
  runningSum = 0.0f;
  amplitudeScalar = 1/(float)NUM_OCTAVES;
  //generate a random value to start for all white noise generators
  for(int i = 0; i < NUM_OCTAVES; i++){
    runningSum += noiseArray[i].generateSample() * amplitudeScalar;
  }
  index = 0;//where are we in the 'whichOctave' logic?
}
PinkNoise::~PinkNoise(){
  delete[] currentBlock;//free the memory 
}
//primary mechanics of class
//=========================================================
float PinkNoise::generateSample(){//generate a single sample
  int whichOctave = getOctave(index);//retrieve octave location (which noise)
  index++;//increment location within pattern for next call
  if(index >= indexMax){index = 0;}//wrap index if needed
  //before adding the new noise value, we must subtract the previous
  runningSum -= noiseArray[whichOctave].getSample();
  //now we replace it with the new sample
  runningSum += noiseArray[whichOctave].generateSample();
  currentSample = runningSum * amplitudeScalar;//scale then assign to current sample
  return currentSample;//return calculated sample
}

float* PinkNoise::generateBlock(){//generate a block of samples
  if(currentBlock != nullptr){//if we don't have a local currentBlock yet, 
    currentBlock = new float[pdlSettings::bufferSize];//create a new array of floats
  }
  for(int i = 0; i < pdlSettings::bufferSize; i++){//for every index in buffer
    currentBlock[i] = generateSample();//generate and assign a sample
  }
  return currentBlock;//return full buffer
}

void PinkNoise::checkGetOctaveLogic(){//function used while designing this class
  unsigned numberOfSteps = pow(2, NUM_OCTAVES)-1;//number intuited from graph in hpp
  //print octaves and steps
  std::cout << "There are " <<  NUM_OCTAVES << 
                 "octaves, requiring " << numberOfSteps << 
                 "steps" << std::endl;
  for(int i = 0; i < numberOfSteps; i++){//scan through each step
    int whichOctave = PinkNoise::getOctave(i);
    std::cout << whichOctave << "|";
    for(int j = 0; j < whichOctave; j++){
      std::cout << "*" ;
    }
    std::cout << "\n";
  }
}
//Getters and setters
//=========================================================
//no setters in this class
float PinkNoise::getSample(){return currentSample;}
float* PinkNoise::getBlock(){return currentBlock;}