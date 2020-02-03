#ifndef WTSquare_hpp
#define WTSquare_hpp

#include <iostream>
#include "pdlSettings.hpp"
#include <cmath>
#include "Interpolation.hpp"

#define NUM_TABLES 10
#define TABLESIZE 2048

class SquareTable{
  private://class members are private by default; added for clarity
  static SquareTable* instance;//store a pointer to an instance of the table
  //The next value is simply 'what frequency would be played if 1 sample of 
  //of the table was played per 1 sample of the audio out.
  float fundamentalFrequency = (float)pdlSettings::sampleRate/float(TABLESIZE);
  SquareTable();//constructor is private, which is unusual 
  ~SquareTable();
  void normalizeTables();//bring tables a range to -1 to 1
  float** table;//storage of the table (two dimmensional array of floats);
  float currentLowestFrequency;//used to keep track of table frequencies
  float* lowFrequencyList;//an array of the base frequencies per table
  float nyquist;//nyquist frequency is 1/2 sr
  
  public:
  static SquareTable* getInstance();//provide access to the single instance of the table
  float** getTable();//return a pointer to (a pointer to) the table
  float* getLowFrequencyList();
  float getFundamentalFrequency();//return table fundamental
  int getTableSize();//return table size (-1)
};

class WTSquare{
  public://everything listed after this is public
  WTSquare();//constructor, defined in the cpp
  WTSquare(float frequency);//option to set frequency on construction
  ~WTSquare();//deconstructor (may be needed to free memory)
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

  SquareTable* instance = SquareTable::getInstance();
  float currentTable;
  float whichTable(float frequency);//input frequency, output which table to read
  float frequency, phase, amplitude;//standard oscillator variables
  float currentSample;//current working sample
  float* currentBlock = nullptr;//current working block of samples
  double phaseIncrement;//extra precision necessary 
};

#endif