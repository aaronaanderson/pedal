#include "pedal/FDNReverb.hpp"

FDNReverb::FDNReverb(){
  //fill the 16x16 matrix by nesting the Householder 4x4 matrix into itself
  for(int x = 0; x < 4; x++){
    for(int y = 0; y < 4; y++){
      float aFourVal = AFour[x][y];
      //multiply this value by every A4 value
      for( int subX = 0; subX < 4; subX++){
        for(int subY = 0; subY < 4; subY++){
          A[x * 4 + subX][y * 4 + subY] = aFourVal * AFour[subX][subY];
        }
      }
    }
  }
  setRoomSize(20.0f);
  setReverbTime(2000.0f, 1300.0f);
}
float FDNReverb::process(float input){
  currentSample *= twoOverN;
  currentSample += input;
  float output = 0.0f;
  for(int i = 0; i < N; i++){
    float matrixOutput = 0;
    for(int j = 0; j < N; j++){
      matrixOutput += nodes[j].delayOutput * A[i][j];
    }
    nodes[i].filterOutput = (nodes[i].gain * matrixOutput) + (nodes[i].pole * nodes[i].filterOutput);
    nodes[i].delayLine.inputSample(nodes[i].filterOutput + currentSample);
    nodes[i].delayOutput = nodes[i].delayLine.getDelayed(samplesToMS(nodes[i].m));
    output += nodes[i].delayOutput;  
  }
  currentSample = output;
  return currentSample;
}
void FDNReverb::calculateNodes(){
  float rDC, rNyquist;
  for(int i = 0; i < N; i++){
    rDC = 1.0f - (6.91f * nodes[i].m)/
                 dCReverbTimeInSamples;
    rNyquist = 1.0f - (6.91f * nodes[i].m)/
                       nyquistReverbTimeInSamples;
    nodes[i].gain = (2.0f * rDC * rNyquist)/
                    (rDC + rNyquist);
    nodes[i].pole = (rDC - rNyquist)/
                    (rDC + rNyquist);
  }
}
void FDNReverb::setReverbTime(float timeDC,float timeNyquist){
  dCReverbTimeInSamples = std::max(msToSamples(timeDC), 0.0f);
  nyquistReverbTimeInSamples = std::max(msToSamples(timeNyquist), 0.0f);
  
  float shortestDelay = roomSize / (340.0f * (1/pdlSettings::sampleRate));
  //set the delay of each node to the nearest co-prime spread linearly from the minimal
  //size to 1.5 x the minimal size
  for(int i = 0; i < N; i++){
    float targetLength = linearInterpolation(i/static_cast<float>(N-1), 0.0f, roomSize, 1.0f, roomSize * 1.5f);
    //use method from page 113 to round multiplier to nearest prime multiple
    float multiplier = std::floor(0.5f + std::log(targetLength)/std::log(PRMIE_BASE));
    nodes[i].m = PRMIE_BASE * multiplier;
  }
  calculateNodes();
}
void FDNReverb::setRoomSize(float newRoomSize){roomSize = newRoomSize;}