#include "pedal/FDNReverb.hpp"

using namespace pedal;

FDNReverb::FDNReverb(){
  //fill the 16x16 matrix by nesting the Householder 4x4 matrix into itself
  for(int x = 0; x < 4; x++){
    for(int y = 0; y < 4; y++){
      float aFourVal = AFour[x][y];
      //multiply this value by every A4 value
      for( int subX = 0; subX < 4; subX++){
        for(int subY = 0; subY < 4; subY++){
          A[x * 4 + subX][y * 4 + subY] = aFourVal * AFour[subX][subY];// * 0.25f;
          //std::cout << "x: " << x * 4 + subX << " y: " << y * 4 + subY << " "  << aFourVal * AFour[subX][subY] << std::endl;
        }
      }
    }
  }
  currentSample = 0.0f;
  setRoomSize(5.0f);
  setReverbTime(2000.0f, 1300.0f);
  std::cout << "FDNReverb not complete" << std::endl;
}
float FDNReverb::process(float input){
  currentSample *= twoOverN;
  currentSample += input;
  float output = 0.0f;
  for(int i = 0; i < N; i++){
    float matrixOutput = 0;
    for(int j = 0; j < N; j++){
      matrixOutput += nodes[j].delayOutput * A[j][i];
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
    std::cout << "node: " << i << " gain: " << nodes[i].gain;
    nodes[i].pole = (rDC - rNyquist)/
                    (rDC + rNyquist);
    std::cout << " pole: " << nodes[i].pole << std::endl;
  }
}
void FDNReverb::setReverbTime(float timeDC,float timeNyquist){
  dCReverbTimeInSamples = std::max(msToSamples(timeDC), 0.0f);
  nyquistReverbTimeInSamples = std::max(msToSamples(timeNyquist), 0.0f);
  
  float shortestDelay = roomSize / (343.0f * (1.0f/Settings::sampleRate));
  //set the delay of each node to the nearest co-prime spread linearly from the minimal
  //size to 1.5 x the minimal size
  for(int i = 0; i < N; i++){
    float targetLength = linearInterpolation(i/static_cast<float>(N-1), shortestDelay,  shortestDelay * 1.5f);
    //use method from page 113 to round multiplier to nearest prime multiple
    float multiplier = std::floor(0.5f + std::log(targetLength)/std::log(PRMIE_BASE));
    nodes[i].m = msToSamples(targetLength * (1.0f + (i/float(N))));//PRMIE_BASE * multiplier;
    //nodes[i].delayLine.setDuration(targetLength * (1.0f + (i/float(N))));
    std::cout << "node " << i << " " << nodes[i].m << std::endl;
  }
  calculateNodes();
}
void FDNReverb::setRoomSize(float newRoomSize){roomSize = newRoomSize;}