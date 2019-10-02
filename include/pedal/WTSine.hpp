//not done

/*
All wave Antialiased wavetables are in these two files. These
classes only generate and provide access to wavetables; they 
do not provide the functionality to play them. This is because
only one wavetable is needed no matter how many instances of the 
oscillator are needed. To achieve antialiasing, 10 distinct versions
of the waveform are stored, one for each octave of the audible hearing
range (20 Hz to 20,000 Hz). Only audible haromincs below the nyquist are 
considered in the waveforms. 
*/

#ifndef WTSine_hpp
#define WTSine_hpp

#include "pdlSettings.hpp"
#include "utilities.hpp"
#include "math.h"

#define TABLESIZE 2048

class SineTable{
    //since the sine table only contains energy at one frequency, 
    //there is no need to stack verisons of tables
  private:
  static SineTable* instance;
  float tableFundamentalFrequency = pdlSettings::sampleRate/float(TABLESIZE);
  SineTable();

  public:

};

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