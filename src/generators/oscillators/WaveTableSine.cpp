#include "pedal/WaveTableSine.hpp"

using namespace pedal;

//Sine Table=====================================================
SineTable::SineTable(){//when it is time to build a table (constructor)
  fundamentalFrequency = (float)Settings::sampleRate / float(getTableSize());
  table = new float[TABLESIZE];//allocate memory for the table
  for(int i = 0; i < TABLESIZE; i++){//for every memory location
    //convert i from 0 - (TABLESIZE-1) to 0 - TWO_PI
    float phase = (i * pedal::TWOPI)/float(TABLESIZE-1);
    //cout << phase << endl;
    table[i] = std::sin(phase);//store the sample in the table
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
int SineTable::getTableSize(){
  //tell the player that the wave table is 1 sample smaller, for linear interpolation
  return TABLESIZE - 1;
}
//initiate the static pointer as a nullptr
SineTable* SineTable::instance = nullptr;

//WaveTableSine==================================================

WaveTableSine::WaveTableSine(){
  setFrequency(440.0f);
  setPhase(0.0f);
  setAmplitude(1.0f);
}
WaveTableSine::WaveTableSine(float initialFrequency){
  setFrequency(initialFrequency);
  setPhase(0.0f);
  setAmplitude(1.0f);
}
WaveTableSine::~WaveTableSine(){}//when object is deleted

float WaveTableSine::generateSample(){
  //interpolate between the prvious and next stored value
  float* table = sineTable->getTable();
  currentSample = linearInterpolation(phase,
                                      table[int(phase)],
                                      table[int(phase+1.0f)]);
  phase += (float)phaseIncrement;//increment phase
  int tableSize = sineTable->getTableSize()-1;//store since we need it often
  if(phase > tableSize){phase -= tableSize; }//if the phase is past the end of the table
  if(phase < 0.0){phase += tableSize;}//if the phase is past the beginning of table (negative frequencies)
  currentSample *= amplitude;//scale result by amplitude
  return currentSample;//return results
}

void WaveTableSine::setFrequency(float newFrequency){
  frequency = newFrequency;
  phaseIncrement = frequency/float(sineTable->getFundamentalFrequency());
}
void WaveTableSine::setPhase(float newPhase){//expecting 0-TWO_PI
  phase = std::fmod(std::fabs(newPhase), pedal::TWOPI);//wrap to 0 -TWO_PI
  float scalar = sineTable->getTableSize()/pedal::TWOPI;
  phase = phase * scalar;//map 0-TWO_PI to 0 - tablSize
}
void WaveTableSine::setAmplitude(float newAmplitude){amplitude = newAmplitude;}

float WaveTableSine::getFrequency(){return frequency;}
float WaveTableSine::getPhase(){
  return (phase * pedal::TWOPI)/float(sineTable->getTableSize());
}
float WaveTableSine::getAmplitude(){return amplitude;}
float WaveTableSine::getSample(){return currentSample;}


