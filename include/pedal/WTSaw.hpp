#ifndef WTSaw_hpp
#define WTSaw_hpp

#include "pdlSettings.hpp"

#define NUM_TABLES 10
#define TABLESIZE 2048

class SawTable{
  private://class members are private by default; added for clarity
  static SawTable* instance;//store a pointer to an instance of the table
  //The next value is simply 'what frequency would be played if 1 sample of 
  //of the table was played per 1 sample of the audio out.
  float fundamentalFrequency = pdlSettings::sampleRate/float(TABLESIZE);
  SawTable();//constructor is private, which is unusual 
  ~SawTable();
  void normalize();//bring a range to -1 to 1
  float** table;//storage of the table (two dimmensional array of floats);
  float currentLowestFrequency;//used to keep track of table frequencies
  static float* lowFrequencyList;//an array of the base frequencies per table
  static float nyquist = pdlSettings::sampleRate * 0.5f;//nyquist frequency is 1/2 sr
  
  public:
  static SawTable* getInstance();//provide access to the single instance of the table
  float** getTable();//return a pointer to (a pointer to) the table
  float getFundamentalFrequency();//return table fundamental
  int getTableSize();//return table size (-1)
};

//using Table = std::array<float, TABLESIZE>;
//std::array<Table, 10> table;

class WTSaw{
  public://everything listed after this is public
  WTSaw();//constructor, defined in the cpp
  WTSaw(float frequency);//option to set frequency on construction
  ~WTSaw();//deconstructor (may be needed to free memory)
  float generateSample();//generate and return a single sample
  float* generateBlock();//generate and return a block of samples

  //"setters"
  void setFrequency(float newFrequency);
  void setPhase(float newPhase);
  void setAmplitude(float newAmplitude);

  //"getters"
  float getFrequency();
  float getPhase();
  float getAmplitude();
  float getSample();
  float* getBlock();
    
  private://everything after this is private (cannot be accessed externally without
  //a "getter" or a "setter"
  //best practice to leave inner workings private

  SawTable* sineTable = SawTable::getInstance();
  float frequency, phase, amplitude;//standard oscillator variables
  float currentSample;//current working sample
  float* currentBlock;//current working block of samples
  double phaseIncrement;//extra precision necessary 
};

#endif