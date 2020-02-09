#ifndef PinkNoise_hpp
#define PinkNoise_hpp

#include <iostream>
#include "math.h"
#include "WhiteNoise.hpp"
#define NUM_OCTAVES 14 

class PinkNoise{
  public:
  PinkNoise();//constructor
  ~PinkNoise();//deconstructor
  float generateSample();//main function
  float* generateBlock();//fills entire buffer
  
  float getSample();//retrieve sample w/o processing
  float* getBlock();//retrieve block w/o processing

  private:
  inline int getOctave(int index){//which whitenoise generator
    int indexLocation = index;//current location within pattern
    int whichOctave = 0;//octave is 0 by default (right 50% of the time)
    while(indexLocation % 2 == 1){//if indexLocation is odd
      whichOctave++;//increment octave
      indexLocation /= 2;//prepare for next comparison
    }
    return whichOctave;//return selected octave
  }
  WhiteNoise noiseArray[NUM_OCTAVES];//array of white noise generators
  unsigned index;//current location in ovtave-retrieval pattern
  unsigned indexMax;//How many steps in octave-retrieval pattern? (depends on how many octaves)
  float runningSum;//Sum of all octaves, updated per sample
  float amplitudeScalar;//ensure the output is between -1.0 and 1.0
  float currentSample;//store for retrieval
  float* currentBlock = nullptr;
  void checkGetOctaveLogic();//Leaving this in for the curious type
};
#endif

/*
This pink noise implementation uses an N-octave
sample-and-hold sum. On each sample, one of the N
WhiteNoise generators generates a new sample. That
generator's previous value is first subtracted from 
a running total, then the new value is added to the
running total.

//1x x x x x x x x x x x x x x x x 2  | 0
//2 x   x   x   x   x   x   x   x  4  | 1
//3   x       x       x       x    8  | 3
//4       x               x        16 | 7 
//5               x                32 | 15
...                                  ...
//12                             4096 | 2047

No matter how many octaves there are, this 
pattern can provide a solution with a series
of steps equal to 2^N. The function .getOctave()
takes this integer index in the pattern and produces
the correct octave to change for that time step.

This approach was adopted from techniques found at this url
http://www.firstpr.com.au/dsp/pink-noise/
*/