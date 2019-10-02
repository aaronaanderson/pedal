//incomplete
#ifndef WTSine_hpp
#define WTSine_hpp

#include "pdlSettings.hpp"
#include "utilities.hpp"
#include "math.h"

#define TABLESIZE 2048
//======================Define a wavetable template
class SineTable{
    //since the sine table only contains energy at one frequency, 
    //there is no need to stack verisons of tables
  private://class members are private by default; added for clarity
  static SineTable* instance;//store a pointer to an instance of the table
  float tableFundamentalFrequency = pdlSettings::sampleRate/float(TABLESIZE);
  SineTable();//constructor is private, which is unusual 
  float* table;//storage of the table;
  public:
  
  static SineTable* getInstance();//provide access to the single instance of the table
  float* getTable();//return a pointer to the table
  
};
//======================Play the table defined above
class WTSine{
  public://everything listed after this is public
  WTSine();//constructor, defined in the cpp
  WTSine(float frequency);//option to set frequency on construction
  ~WTSine();//deconstructor (may be needed to free memory)
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

  SineTable* sineTable = SineTable::getInstance();
  float frequency, phase, amplitude;//standard oscillator variables
  float currentSample;//current working sample
  float* currentBlock;//current working block of samples
  double phaseIncrement;//extra precision necessary 
};
#endif