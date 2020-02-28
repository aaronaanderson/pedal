#ifndef RMS_HPP
#define RMS_HPP

#include "CircularBuffer.hpp"
#include "utilities.hpp"

class RMS{
  public:
  RMS(int samplePeriod = 64);
  float process(float input);
  private:
  CircularBuffer buffer;//keep track of data to average
  SmoothValue currentSample;//output only calculated every N samples, so smooth
  int samplesToAverage;//how many samples in an average?
  int sampleCounter;//to keep track of how many we've added

};
#endif