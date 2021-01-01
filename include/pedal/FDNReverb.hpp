#ifndef FDNReverb_hpp
#define FDNReverb_hpp
//not ready
#include <cmath>
#include <algorithm>
#include "pedal/utilities.hpp"
#include "pedal/CircularBuffer.hpp"
#include "pedal/Interpolation.hpp"
#define N 16
#define PRMIE_BASE 7

namespace pedal{
struct node{
  float m;//delay in samples
  float delayOutput=0.0f;
  float gain;
  float pole;
  float filterOutput=0.0f;
  CircularBuffer delayLine = CircularBuffer(20000.0f);
};
class FDNReverb{
  public:
  FDNReverb();
  float process(float input);

  void setReverbTime(float timeDC, float timeNyquist);
  void setRoomSize(float newRoomSize);//in meters
  float getSample();

  private:
  void calculateNodes();
  float currentSample;
  float dCReverbTimeInSamples;
  float nyquistReverbTimeInSamples;
  float roomSize;//aka 'average distance before obstruction'
  node nodes[N];
  const float AFour[4][4] = {{0.5f, -0.5f, -0.5f, -0.5f},
                            {-0.5f, 0.5f, -0.5f, -0.5f}, 
                            {-0.5f, -0.5f, 0.5f, -0.5f},
                            {-0.5f, -0.5f, -0.5f, 0.5f}};
  float A[N][N];
                  
  const float twoOverN = -2.0f / static_cast<float>(N);//2/N
};
}//end pedal namespace
#endif