#include "pedal/WTSaw.hpp"

//Sine Table=====================================================
SawTable::SawTable(){//when it is time to build a table (constructor)
  currentLowestFrequency = 20.0f;//20 will be considered the lowest frequency;
  fundamentalFrequency = pdlSettings::sampleRate/float(TABLESIZE - 1);
  lowFrequencyList = new float[NUM_TABLES];
  for(int i = 0; i < NUM_TABLES; i++){
    lowFrequencyList = currentLowestFrequency;
    currentLowestFrequency *= 2.0f;//20,40,80,160,320,640,1280,2560,5120,10240
  }
}

SawTable::~SawTable(){//deconstructor (what to do when done)
  delete[] lowFrequencyList;
  for(int i = 0; i < NUM_TABLES; i++){
    delete[] table[i];
  }
  delete[] table;
}

//since the construct was private, an access method must be included
SawTable* SawTable::getInstance(){
  if(instance == nullptr){
    instance = new SawTable();
  }
  return instance;
}
//access to the data in the table
float** SawTable::getTable(){return table;}
float SawTable::getFundamentalFrequency(){return fundamentalFrequency;}
int SawTable::getTableSize(){
  //tell the player that the wave table is 1 sample smaller, for linear interpolation
  return TABLESIZE - 1;
}
//initiate the static pointer as a nullptr
SawTable* SawTable::instance = nullptr;

//WaveTableSine==================================================

WTSaw::WTSaw(){
  setFrequency(440.0f);
  setPhase(0.0f);
  setAmplitude(1.0f);
}
WTSaw::WTSaw(float initialFrequency){
  setFrequency(initialFrequency);
  setPhase(0.0f);
  setAmplitude(1.0f);
}
WTSaw::~WTSaw(){
  delete[] currentBlock;
}

float WTSaw::generateSample(){

  return currentSample;//return results
}

float* WTSaw::generateBlock(){
  if(currentBlock == nullptr){//if the block hasn't been allocated
    currentBlock = new float[pdlSettings::bufferSize];//allocate the block
  }
  for(int i= 0; i < pdlSettings::bufferSize; i++){//for every sample in block
    currentBlock[i] = generateSample();//assign the next sample
  }
}

void WTSaw::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = frequency/float(sineTable->getFundamentalFrequency());
}
void WTSaw::setPhase(float newPhase){//expecting 0-TWO_PI
  phase = fmod(fabs(newPhase), 6.2831853072);//wrap to 0 -TWO_PI
  float scalar = sineTable->getTableSize()/6.2831853072;
  phase = phase * scalar;//map 0-TWO_PI to 0 - tablSize
}
void WTSaw::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float WTSaw::getFrequency(){return frequency;}
float WTSaw::getPhase(){
  return (phase * 6.2831853072)/float(sineTable->getTableSize());
}
float WTSaw::getAmplitude(){return amplitude;}
float WTSaw::getSample(){return currentSample;}
float* WTSaw::getBlock(){return currentBlock;}


