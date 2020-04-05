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

#include "pedal/Window.hpp"
#include "pedal/MicroBenchmark.hpp"
/*
STFT, or Short-Time Fourier Transform, converts
an incoming signal from time domain to frequency
domain.
*/
class STFT{
  public:
  STFT(int initialWindowSize = 512, int initialOverlap = 4);
  bool updateInput(float input);//return true if full frame is ready
  bool isFFTReady();//are bins ready for manipulation?
  float updateOutput();//update system
  
  void setWindowType(Window::Mode newWindowType);//not safe to call during analysis
  void setOverlap(int newOverlap);
  void setWindowSize(int powOfTwoFFTSize);//must be a power of two(will be adjusted if not)
  void setBin(int whichBin, std::complex<float> complexInput);
  void setBinMagnitude(int whichBin, float newMagnitude);
  void setBinPhase(int whichBin, float newPhase);
  float getCurrentSample();//get output without udpating
  Window::Mode getWindowType();
  int getWindowSize();
  int getOverlap();
  int getHopSize();//windowSize / overlap
  int getNumberOfBins();//windowSize / 2
  std::complex<float> getBin(int whichBin);//get fft output at whichBin
  float getBinMagnitude(int whichBin);// get bin magnitude, calculated at tall
  float getBinPhase(int whichBin); //get bin phase, calculated at call
  float getBinFrequency(int whichBin);//get bin frequency, calculated at call
  float* getRealBufferPointer();// get pointer to real array (size windowSize)
  float* getImaginaryBufferPointer();// get pointer to imaginary array (size windowSize)

  private:
  float currentSample;
  void calculateWindow();//generate stored window data
  inline void calculateWindowedInput(const int inputOffset);
  int hopSize;//how many new samples before next analysis?
  int overlap;//how many analyses in the span of one window
  int windowSize;//FFT analysis size (and size of many other data)
  Window::Mode windowType;//store current window type
  int whichInputSegment;//used to offset write position
  int whichOutputLayer;//used to offset write position
  bool fftReadyFlag;//bins may be updated if so. iFFT is called next sample
  int inputWritePosition;//write location within input buffer
  int inputWriteRedundantPosition;//duplicate entry for array alignment
  std::vector<float> inputBuffer;//time-domain input stream
  std::vector<float> windowedInputSegment;//FFT input
  std::vector<float> realBuffer;//real results of FFT
  std::vector<float> imaginaryBuffer;//imaginary results of FFT
  std::vector<float> window;//store window locally
  std::vector<std::vector<float>> windowedOutput;//updated every 'hopSize' samples
  int currentOutputIndex;//which sample in the overlapAddOutput buffer
  audiofft::AudioFFT fft;//fast fourier transform (from external audioFFT library)
};

inline void STFT::calculateWindowedInput(const int inputOffset){
  for(int i = 0; i < windowSize; i ++){
    windowedInputSegment[i] = inputBuffer[i + inputOffset] * window[i];
  }
}
#endif