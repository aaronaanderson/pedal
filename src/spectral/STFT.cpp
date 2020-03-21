#include "pedal/STFT.hpp"

STFT::STFT(int initialWindowSize){
  setWindowType(WindowTypes::HANNING);
  setWindowSize(initialWindowSize);
  

  fft.init(windowSize);
  // std::cout << "c" << std::endl;
  overlap = 8;
  hopSize = windowSize / overlap;
  fftReadyFlag = false;
  inputWritePosition = 0;
  whichInputSegment = 0;//keep track of which segment to use ('overlap' total segments)
  whichOutputSegment = 0;//same for output writing (system is slightly different, however)
  currentOutputIndex = 0;
  currentSample = 0.0f;//not used unless inversing
  
}

bool STFT::updateInput(float input){
  inputBuffer[inputWritePosition] = input;
  //redundant write for clean array access later
  inputBuffer[inputWriteRedundantPosition] = input;
  inputWritePosition++;//increment write position
  inputWritePosition = inputWritePosition % windowSize;//wrap at window size
  //the redundant write is 'windowSize' samples ago, wrapped around the inputBuffer size
  inputWriteRedundantPosition = (inputWritePosition + windowSize) % inputBuffer.size();
  if(inputWritePosition % hopSize == 0){//if an input segment has been filled
    const int offset = hopSize * whichInputSegment;
    calculateWindowedInput(offset);//scale input segment by window, assign to windowedInputSegment
    //this is the expensive line
    //realBuffer and imaginaryBuffer are updated in place, ready for use after this call
    fft.fft(windowedInputSegment.data(), realBuffer.data(), imaginaryBuffer.data());
    //ff
    fftReadyFlag = true;
    //increment and wrap which input segment is being pointed to
    whichInputSegment++;
    whichInputSegment = whichInputSegment % overlap;
  }else{
    fftReadyFlag = false;
  }
  return fftReadyFlag;
}
float STFT::updateOutput(){//MUST be called per sample if at all
  if(fftReadyFlag){//segment is ready to be ifftd and added
    
    //zero the previous 'hopSize' samples
    
    fft.ifft(outputBuffer.data(), realBuffer.data(), imaginaryBuffer.data());
    const int offset = hopSize * whichOutputSegment;
    calculateWindowedOutput(offset);//window results, then overlap and add
    whichOutputSegment++;//increment to the next segment
    whichOutputSegment = whichOutputSegment % overlap;//wrap if over
  }
  currentOutputIndex = currentOutputIndex % windowSize;//wrap output index
  currentSample = overlapAddOutput[currentOutputIndex];//grab from precalculated value
  currentOutputIndex++;//increment output buffer index
  return currentSample;//return single sample
}
std::complex<float> STFT::getBin(int whichBin){//git bin's real and imaginary components
  std::complex<float> bin;//temporary bin 
  //assign from calculated fft
  bin.real(realBuffer[whichBin]);
  bin.imag(imaginaryBuffer[whichBin]);
  return bin;
}
//user is responsible for not asking for a bin that doesn't exist(which bin shouldn't be > windowSize-1)
void STFT::setBin(int whichBin, std::complex<float> complexInput){
  realBuffer[whichBin] = std::real(complexInput);
  imaginaryBuffer[whichBin] = std::imag(complexInput);
}
void STFT::setWindowType(WindowTypes newWindowType){
  if(newWindowType != windowType){
    windowType = newWindowType;
    calculateWindow();
  }
}
void STFT::setWindowSize(int powerOfTwoSize){
  //round up to nearest power of 2
  //from stack overflow:https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
  windowSize = std::pow(2, std::ceil(std::log(powerOfTwoSize)/
                                     std::log(2.0f)));
  inputBuffer.resize(windowSize + (hopSize * (overlap - 1)));//*see notes at bottom
  windowedInputSegment.resize(windowSize);
  realBuffer.resize(windowSize);
  imaginaryBuffer.resize(windowSize);
  outputBuffer.resize(windowSize);
  window.resize(windowSize);
  overlapAddOutput.clear();//clear to ensure all values will be 0 in next line 
  overlapAddOutput.resize(windowSize, 0.0f);//resize and fill with 0.0f
  calculateWindow();                        
}

void STFT::calculateWindow(){
  float phaseReciprocal = 1.0f/static_cast<float>(windowSize);
  for(int i = 0; i < windowSize; i++){
    float phase = i * phaseReciprocal;
    window[i] = HanningWindow::sampleFromPhase(phase);
    std::cout << window[i] << std::endl;
  }
}
bool STFT::isFFTReady(){return fftReadyFlag;}
int STFT::getNumberOfBins(){return windowSize/2;}