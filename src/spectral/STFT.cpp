#include "pedal/STFT.hpp"

STFT::STFT(int initialWindowSize){
  setWindowSize(initialWindowSize);
  hopSize = windowSize / overlap;
}

void STFT::setWindowSize(int powerOfTwoSize){
  //round up to nearest power of 2
  //from stack overflow:https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
  windowSize = std::pow(2, std::ceil(std::log(powerOfTwoSize)/
                                     std::log(2.0f)));
  window.resize(windowSize);
  realBuffer.resize(windowSize);
  imaginaryBuffer.resize(windowSize);
  inputBuffer.resize(windowSize);
  outputBuffer.resize(windowSize);
  calculateWindow();                        
}

void STFT::calculateWindow(){
  
  for(int i = 0; i < windowSize; i++){

  }
}