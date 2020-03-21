#ifndef STFT_hpp
#define STFT_hpp
/*
Short-Time Fourier Transform
windowed overlap+add FFT
*/
#include "../../AudioFFT/AudioFFT.h"//external FFT library
#include <complex>//std complex data type
#include <vector>//for dynamic arrays
#include <cmath>
#include <iostream>

#include "pedal/HanningWindow.hpp"
enum WindowTypes{
  HANNING = 0, 
  HAMMING, 
  BLACKMANHARRIS
};
/*
STFT, or Short-Time Fourier Transform, converts
an incoming signal from time domain to frequency
domain.
*/
class STFT{
  public:
  STFT(int initialWindowSize = 512);
  bool updateInput(float input);//return true if full frame is ready
  bool isFFTReady();
  float updateOutput();
  float getCurrentSample();

  void setWindowType(WindowTypes newWindowType);//not safe to call during analysis
  void setOverlap(float newOverlap);
  void setWindowSize(int powOfTwoFFTSize);//must be a power of two(will be adjusted if not)
  void setBin(int whichBin, std::complex<float> complexInput);
  int getWindowType();
  int getOverlap();
  int getWindowSize();
  int getNumberOfBins();
  std::complex<float> getBin(int whichBin);
  
  private:
  float currentSample;
  void calculateWindow();//generate stored window data
  inline void calculateWindowedInput(const int inputOffset);
  inline void calculateWindowedOutput(const int outputOffset);
  int hopSize;//how many new samples before next analysis?
  int overlap;//how many analyses in the span of one window
  int windowSize;//FFT analysis size (and size of many other data)
  WindowTypes windowType;//store current window type
  int whichInputSegment;//used to offset write position
  int whichOutputSegment;//used to offset write position
  bool fftReadyFlag;//bins may be updated if so. iFFT is called next sample
  int inputWritePosition;//write location within input buffer
  int inputWriteRedundantPosition;//duplicate entry for array alignment
  std::vector<float> inputBuffer;//time-domain input stream
  std::vector<float> windowedInputSegment;//FFT input
  std::vector<float> realBuffer;//real results of FFT
  std::vector<float> imaginaryBuffer;//imaginary results of FFT
  std::vector<float> outputBuffer;//inverse-fft results
  std::vector<float> window;//store window locally
  std::vector<float> overlapAddOutput;//updated every 'hopSize' samples
  int currentOutputIndex;//which sample in the overlapAddOutput buffer
  audiofft::AudioFFT fft;//fast fourier transform (from external audioFFT library)
};
inline void STFT::calculateWindowedInput(const int inputOffset){
  for(int i = 0; i < windowSize; i += 4){//unroll 4 at a time for cache/memory optimization
    windowedInputSegment[i] = inputBuffer[i + inputOffset] * window[i];
    windowedInputSegment[i+1] = inputBuffer[i + inputOffset + 1] * window[i+1];
    windowedInputSegment[i+2] = inputBuffer[i + inputOffset + 2] * window[i+2];
    windowedInputSegment[i+3] = inputBuffer[i + inputOffset + 3] * window[i+3];
  }
}
inline void STFT::calculateWindowedOutput(const int outputOffset){
  //clear previous 'hopsize' samples (their values are no longer needed)
  int startIndex = ((outputOffset - hopSize) + windowSize) % windowSize;
  //std::cout << startIndex << std::endl;
  for(int i = 0; i < hopSize; i++){
    overlapAddOutput[startIndex + hopSize];
  }
  //overlap add windowed iFFT result
  for(int i = 0; i < windowSize; i += 4){//unroll 4 at a time for cache/memory optimization
    overlapAddOutput[(i + outputOffset) % windowSize] += outputBuffer[i] * window[i];
    overlapAddOutput[(i + outputOffset + 1) % windowSize] += outputBuffer[i+1] * window[i+1];
    overlapAddOutput[(i + outputOffset + 2) % windowSize] += outputBuffer[i+2] * window[i+2];
    overlapAddOutput[(i + outputOffset + 3) % windowSize] += outputBuffer[i+3] * window[i+3];
  }
}
#endif