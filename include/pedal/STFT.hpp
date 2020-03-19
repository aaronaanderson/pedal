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

#include "pedal/HanningWindow.hpp"
enum WindowTypes{
  HANNING = 0, 
  HAMMING, 
  BLACKMANHARRIS
};

class STFT{
  public:
  STFT(int initialWindowSize = 512);
  bool update(float input);//return true if full frame is ready
  bool isWindowReady();
  
  void setWindowType(WindowTypes newWindowType);//not safe to call during analysis
  void setOverlap(float newOverlap);
  void setWindowSize(int powOfTwoFFTSize);//must be a power of two(will be adjusted if not)
  void setBin(int whichBin, std::complex<float> complexInput);
  int getWindowType();
  int getOverlap();
  int getWindowSize();
  std::complex<float> getBin(int whichBin);
  
  private:
  void calculateWindow();
  int hopSize;
  int overlap;
  int windowSize;
  bool isFrameReady;

  std::vector<float> realBuffer;
  std::vector<float> imaginaryBuffer;
  std::vector<float> inputBuffer;
  std::vector<float> outputBuffer;
  std::vector<float> window;
};
#endif