#include "pedal/WTSine.hpp"

//Sine Table=====================================================
SineTable::SineTable(){//when it is time to build a table (constructor)
  table = new float[TABLESIZE];//allocate memory for the table
  for(int i = 0; i < TABLESIZE; i++){//for every memory location
    //convert i from 0 - (TABLESIZE-1) to 0 - TWO_PI
    float phase = (i * TABLESIZE * 6.2831853072)/float(TABLESIZE);
    table[i] = sin(phase);//store the sample in the table
  }
}

SineTable::~SineTable(){//deconstructor (what to do when done)
    if(table != nullptr){//if the table has allocated memory
        delete[] table;//free the memory
    }
}

//since the construct was private, an access method must be included
SineTable* SineTable::getInstance(){
  if(instance == nullptr){
    instance = new SineTable();
  }
  return instance;
}
//access to the data in the table
float* SineTable::getTable(){return table;}
float SineTable::getFundamentalFrequency(){return fundamentalFrequency;}
//initiate the static pointer as a nullptr
SineTable* SineTable::instance = nullptr;//@kee is this necessary?

//WaveTableSine==================================================

WTSine::WTSine(){
  setFrequency(440.0f);
  setPhase(0.0f);
  setAmplitude(1.0f);
}
WTSine::WTSine(float initialFrequency){
  setFrequency(initialFrequency);
  setPhase(0.0f);
  setAmplitude(1.0f);
}
WTSine::~WTSine(){
  if(currentBlock != nullptr){
      delete[] currentBlock;
  }
}

float WTSine::generateSample(){
  //interpolate between the prvious and next stored value
  currentSample = linearInterpolation(phase,
                                      SineTable->getTable()[int(phase)],
                                      SineTable->getTable()[int(phase+1.0f)]));
  phase += phaseIncrement;//increment phase
  int tableSize = SineTable->getTableSize()-1;//store since we need it often
  if(phase >= tableSize){phase -= tableSize; }//if the phase is past the end of the table
  if(phase <= 0.0){phase += tableSize;}//if the phase is past the beginning of table (negative frequencies)
  currentSample *= amplitude;//scale result by amplitude
  return currentSample;//return results
}

float* WTSine::generateBlock(){
  if(currentBlock == nullptr){//if the block hasn't been allocated
    currentBlock = float[pdlSettings::bufferSize];//allocate the block
  }
  for(int i= 0; i < pdlSettings::bufferSize; i++){//for every sample in block
    currentBlock[i] = generatSample();//assign the next sample
  }
}

void WTSine::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = frequency/float(sineTable->getFundamentalFrequency());
}
void WTSine::setPhase(float newPhase){//expecting 0-TWO_PI
  phase = fmod(fabs(newPhase), 6.2831853072);//wrap to 0 -TWO_PI
  float scalar = SineTable->getTableSize()/6.2831853072;
  phase = phase * scalar;//map 0-TWO_PI to 0 - tablSize
}
void WTSine::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float WTSine::getFrequency(){return frequency;}
float WTSine::getPhase(){
  return (phase * 6.2831853072)/float(SineTable->getTableSize());
}
float WTSine::getAmplitude(){return amplitude;}
float WTSine::getSample(){return currentSample;}
float* WTSine::getBlock(){return currentBlock;}


