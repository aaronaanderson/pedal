#include "pedal/WTSaw.hpp"

//Sine Table=====================================================
SawTable::SawTable(){//when it is time to build a table (constructor)
  currentLowestFrequency = 20.0f;//20 will be considered the lowest frequency;
  fundamentalFrequency = pdlSettings::sampleRate/float(TABLESIZE - 1);
  lowFrequencyList = new float[NUM_TABLES];//allocate space for this list
  //allocate space for the 2D array of tables
  table = new float*[NUM_TABLES];//allocate each table slot
  for(int i = 0; i < NUM_TABLES; i++){//for each table slot,
    table[i] = new float[TABLESIZE];//allocate space for the table
    for(int j = 0; j < TABLESIZE; j++){//for every sample in the table
      table[i][j] = 0.0f;//0 the memory
    }
  }
  
  for(int i = 0; i < NUM_TABLES; i++){//for each table 
    lowFrequencyList[i] = currentLowestFrequency;//keep track of the lowest frequency per octave
    currentLowestFrequency *= 2.0f;//advance to the next octave
    //20,40,80,160,320,640,1280,2560,5120,10240
  }
  
  //generate the harmonics
  for(int i = 0; i < NUM_TABLES; i++){//for each table
    //how many haromincs are available before aliasing?
    //add harmonics while under the nyquist frequency
    int availableHarmonics = 1;//start with a single available harmonic
    while(lowFrequencyList[i]*(availableHarmonics + 1) < nyquist){//test va
      availableHarmonics += 1;
      if(availableHarmonics > getTableSize() * 0.5){
        break;//can't store harmonics greater than 1/2 the fundamental 
      }
    }
    
    for(int j = 0; j < TABLESIZE; j++){//for each sample in that table
      for(int harmonic = 1;harmonic < availableHarmonics; harmonic++){//for each available harmonic
        float phase = (j * 6.2831853 * harmonic)/float(getTableSize());
        float harmonicAmplitude = -1.0f/float(harmonic);
        table[i][j] += sin(phase) * harmonicAmplitude;
      }
    }
    //normalize the table
    normalize();
  }
}

SawTable::~SawTable(){//deconstructor (what to do when done)
  delete[] lowFrequencyList;
  for(int i = 0; i < NUM_TABLES; i++){
    delete[] table[i];
  }
  delete[] table;
}

void SawTable::normalizeTables(){
  for(int i = 0; i < NUM_TABLES; i++){//for each table
    float largestValue = 0.0f;//TODO this should really be -inf
    //find the largest value in the buffer
    for(int j = 0; j < TABLESIZE; j++){
      largestValue = fmax(fabs(inputBuffer[i]), largestValue);
    }
    //Do some math, largestValue*scalarValue=1.0
    float scalarValue = 1.0f/largestValue;
    //scale the entire buffer uniformly by this scalar value
    for(int j = 0; j < bufferSize; j++){
      table[i][j] *= scalarValue;
    } 
  }
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

//Constructors and Deconstructors=========
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
//Basic Functionallity of class=========
float* WTSaw::generateBlock(){
  if(currentBlock == nullptr){//if the block hasn't been allocated
    currentBlock = new float[pdlSettings::bufferSize];//allocate the block
  }
  for(int i= 0; i < pdlSettings::bufferSize; i++){//for every sample in block
    currentBlock[i] = generateSample();//assign the next sample
  }
}

//Getters and Setters==================
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


