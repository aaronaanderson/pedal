#include "pedal/WTTriangle.hpp"

//Sine Table=====================================================
TriangleTable::TriangleTable(){//when it is time to build a table (constructor)
  nyquist = (float)pdlSettings::sampleRate * 0.5f;
  fundamentalFrequency = (float)pdlSettings::sampleRate/float(TABLESIZE - 1);
  lowFrequencyList = new float[NUM_TABLES];//allocate space for this list
  currentLowestFrequency = 20.0f;//20 will be considered the lowest frequency;
  for(int i = 0; i < NUM_TABLES; i++){//for each table 
    lowFrequencyList[i] = currentLowestFrequency;//keep track of the lowest frequency per octave
    currentLowestFrequency *= 2.0f;//advance to the next octave
    //20,40,80,160,320,640,1280,2560,5120,10240
  }

  //allocate space for the 2D array of tables
  table = new float*[NUM_TABLES];//allocate each table slot
  for(int i = 0; i < NUM_TABLES; i++){//for each table slot,
    table[i] = new float[TABLESIZE];//allocate space for the table
    for(int j = 0; j < TABLESIZE; j++){//for every sample in the table
      table[i][j] = 0.0f;//0 the memory   
    }
  }
    
  //generate the harmonics
  for(int i = 0; i < NUM_TABLES; i++){//for each table
    //how many haromincs are available before aliasing?
    //add harmonics while under the nyquist frequency
    int availableHarmonics = 1;//start with a single available harmonic
    while(lowFrequencyList[i]*(availableHarmonics + 1) < nyquist){//test va
      if(availableHarmonics > getTableSize() * 0.5){//check the table nyquist
        break;//can't store harmonics greater than 1/2 the fundamental 
      }
      availableHarmonics += 1;//add the next harmonic
    }
    
    for(int j = 0; j < TABLESIZE; j++){//for each sample in that table
      table[i][j] = 0.0f;
      for(int harmonic = 1;harmonic < availableHarmonics; harmonic+=2){//for each available harmonic
        float harmonicPhase = (j * 6.2831853f * harmonic)/float(getTableSize());
        float harmonicAmplitude = 1.0f/float(harmonic*harmonic);
        table[i][j] += std::cos(harmonicPhase) * harmonicAmplitude;
      }
    }
    //normalize the table
    normalizeTables(); 
  }

  //normalize the table
  normalizeTables(); 
}

TriangleTable::~TriangleTable(){//deconstructor (what to do when done)
  delete[] lowFrequencyList;
  for(int i = 0; i < NUM_TABLES; i++){
    delete[] table[i];
  }
  delete[] table;
}

void TriangleTable::normalizeTables(){
  for(int i = 0; i < NUM_TABLES; i++){//for each table
    float largestValue = 0.0f;//TODO this should really be -inf
    //find the largest value in the buffer
    for(int j = 0; j < TABLESIZE; j++){
      largestValue = fmax(fabs(table[i][j]), largestValue);
    }
    //Do some math, largestValue*scalarValue=1.0
    float scalarValue = 1.0f/largestValue;
    //scale the entire buffer uniformly by this scalar value
    for(int j = 0; j < TABLESIZE; j++){
      table[i][j] *= scalarValue;
    } 
  }
}

float* TriangleTable::getLowFrequencyList(){return lowFrequencyList;}
//since the construct was private, an access method must be included
TriangleTable* TriangleTable::getInstance(){
  if(instance == nullptr){
    instance = new TriangleTable();
  }
  return instance;
}
//access to the data in the table
float** TriangleTable::getTable(){return table;}
float TriangleTable::getFundamentalFrequency(){return fundamentalFrequency;}
int TriangleTable::getTableSize(){
  //tell the player that the wave table is 1 sample smaller, for linear interpolation
  return TABLESIZE - 1;
}
//initiate the static pointer as a nullptr
TriangleTable* TriangleTable::instance = nullptr;

//WaveTableSine==================================================
//Constructors and Deconstructors=========
WTTriangle::WTTriangle(){
  setFrequency(440.0f);
  setPhase(0.0f);
  setAmplitude(1.0f);
}
WTTriangle::WTTriangle(float initialFrequency){
  setFrequency(initialFrequency);
  setPhase(0.0f);
  setAmplitude(1.0f);
}
WTTriangle::~WTTriangle(){
  delete[] currentBlock;
}
//Basic Functionallity of class==========
float WTTriangle::generateSample(){
  float** table = instance->getTable();
  // store which tables will be used (y axis of 2D array)
  const int lowTableIndex = (int)currentTable;
  const int highTableIndex = (lowTableIndex < NUM_TABLES-1)? lowTableIndex+1 : lowTableIndex; 
  // access the necessary tables from the list of tables
  const float* lowTable = table[lowTableIndex];
  const float* highTable = table[highTableIndex];
  // find the phase (x axis of 2D array)
  const int previousIndex = (int)phase;
  const int nextIndex = (previousIndex + 1) % TABLESIZE; // wrap phase idx
  //this is a process known as bilinear interpolation (2D linear interpolation)
  const float s0 = linearInterpolation(phase, lowTable[previousIndex], lowTable[nextIndex]);
  const float s1 = linearInterpolation(phase, highTable[previousIndex], highTable[nextIndex]);
  currentSample = linearInterpolation(currentTable, s0, s1);;
  currentSample *= amplitude;//scale for amplitude
  phase += (float)phaseIncrement;//progress phase
  int tableSize = instance->getTableSize()-1;//account for the extra sample
  if(phase >= tableSize){phase -= tableSize;}                 
  if(phase <= 0.0f){phase += tableSize;}
  return currentSample;//return results
}

float* WTTriangle::generateBlock(){
  if(currentBlock == nullptr){//if the block hasn't been allocated
    currentBlock = new float[pdlSettings::bufferSize];//allocate the block
  }
  for(int i= 0; i < pdlSettings::bufferSize; i++){//for every sample in block
    currentBlock[i] = generateSample();//assign the next sample
  }
  return currentBlock;
}
float WTTriangle::whichTable(float testFrequency){//essentially the Y value of a 2D array
  float* frequencyList = instance->getLowFrequencyList();//get the list of table frequencies
  //boundry check
  if(testFrequency > frequencyList[NUM_TABLES-1]){//if the frequency is > than the highest table
    currentTable = NUM_TABLES-1;
    return float(currentTable);
  }else if(testFrequency < frequencyList[0]){//if the frequency is < than the lowest table
    currentTable = 0.0f;
    return 0.0f;
  }
  int i = 0;//create an index
  while(testFrequency > frequencyList[i]){//eventually find a fit
    if(testFrequency < frequencyList[i+1]){//if the next lowestFrequency is too high
      float positionBetweenTables = ((testFrequency/frequencyList[i]) - 1.0f);//(0.0 - 1.0)
      return i + positionBetweenTables;
    }else{//if the next increment is not too hight
      i++;//increase the increment
    }
  }
  return 0;//windows compilers force this to be here
}
//Getters and Setters==================
void WTTriangle::setFrequency(float newFrequency){
  frequency = newFrequency;
  currentTable = whichTable(frequency);
  phaseIncrement = frequency/float(instance->getFundamentalFrequency());
}
void WTTriangle::setPhase(float newPhase){//expecting 0-TWO_PI
  phase = std::fmod(std::fabs(newPhase), 6.2831853072f);//wrap to 0 -TWO_PI
  float scalar = instance->getTableSize()/6.2831853072f;
  phase = phase * scalar;//map 0-TWO_PI to 0 - tablSize
}
void WTTriangle::setAmplitude(float newAmplitude){amplitude = newAmplitude;}
float WTTriangle::getFrequency(){return frequency;}
float WTTriangle::getPhase(){
  return (phase * 6.2831853072f)/float(instance->getTableSize());
}
float WTTriangle::getAmplitude(){return amplitude;}
float WTTriangle::getSample(){return currentSample;}
float* WTTriangle::getBlock(){return currentBlock;}


