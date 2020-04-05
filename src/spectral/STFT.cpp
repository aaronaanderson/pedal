#include "pedal/STFT.hpp"

STFT::STFT(int initialWindowSize, int initialOverlap){
  overlap = initialOverlap;
  setWindowType(Window::Mode::HANNING);
  setWindowSize(initialWindowSize);
  fft.init(windowSize);
  hopSize = windowSize / overlap;
  fftReadyFlag = false;
  inputWritePosition = 0;
  inputWriteRedundantPosition = inputWritePosition + windowSize;
  currentOutputIndex = 0;
  currentSample = 0.0f;//not used unless inversing
}
bool STFT::updateInput(float input){
  inputBuffer[inputWritePosition] = input;
  //redundant write for clean array access later
  inputBuffer[inputWriteRedundantPosition] = input;
  inputWritePosition++;//increment write position
  inputWritePosition = inputWritePosition % inputBuffer.size();//wrap at window size
  //the redundant write is 'windowSize' samples ago, wrapped around the inputBuffer size
  inputWriteRedundantPosition = (inputWritePosition + windowSize) % inputBuffer.size();
  //check if there is enough new data for analysis
  if(inputWritePosition % hopSize == 0){//if an input segment has been filled
    const int offset = inputWritePosition % windowSize;//safe because of if check
    calculateWindowedInput(offset);//scale input segment by window, assign to windowedInputSegment
    //realBuffer and imaginaryBuffer are updated in place, ready for use after this call
    fft.fft(windowedInputSegment.data(), realBuffer.data(), imaginaryBuffer.data());
    fftReadyFlag = true;//allow bin manipulation, and tell output resynthesis is ready
    //increment and wrap which input segment is being pointed to
  }else{
    fftReadyFlag = false;//still need more samples for analysis
  }
  return fftReadyFlag;
}
bool STFT::isFFTReady(){
  return fftReadyFlag;
}
float STFT::updateOutput(){//MUST be called per sample if at all
  currentOutputIndex = currentOutputIndex % windowSize;//wrap output index
  if(fftReadyFlag){//segment is ready to be ifftd and windowed
    whichOutputLayer = currentOutputIndex / hopSize;
    //deliver resynthesis to windowedOutput[whichOutputLayer]    
    fft.ifft(windowedOutput[whichOutputLayer].data(), realBuffer.data(), imaginaryBuffer.data());
    //window those samples in place
    for(int i = 0; i < windowSize; i++){
      windowedOutput[whichOutputLayer][i] *= window[i];
    }
  }
  currentSample = 0.0f;//reset currentSample from last round
  for(int i = 0; i < overlap; i++){//for every overlap layer
    //for each itteration, look back an additional 'hopSize' samples
    int index = (currentOutputIndex - (hopSize * i) + windowSize) % windowSize;//add
    currentSample += windowedOutput[i][index];
  }
  currentOutputIndex++;//increment output buffer index
  return currentSample;//return single sample
}
//------------------------------Set/Get
void STFT::setWindowType(Window::Mode newWindowType){
  if(newWindowType != windowType){
    windowType = newWindowType;
    calculateWindow();
  }
}
void STFT::setOverlap(int newOverlap){
  if(newOverlap > overlap){//if adding layers
    //determine how many layers to add
    int numberOfNewLayers = newOverlap - overlap;
    //make an vector of size windowSize filled with 0.0f
    std::vector<float> temp(windowSize, 0.0f);
    //for each new layer
    for(int i = 0; i < numberOfNewLayers; i++){
      //add the new layer
      windowedOutput.push_back(temp);
    }
  }else if(newOverlap < overlap){//if removing layers
    //determine how many layers to remove
    int numberToRemove = overlap - newOverlap;
    //for each layer to remove
    for(int i = 0; i < numberToRemove; i++){
      //remove a layer
      windowedOutput.pop_back();
    }
  }
  overlap = newOverlap;//assign the new overlap value
}
void STFT::setWindowSize(int powerOfTwoSize){
  //round up to nearest power of 2
  //from stack overflow:https://stackoverflow.com/questions/466204/rounding-up-to-next-power-of-2
  windowSize = std::pow(2, std::ceil(std::log(powerOfTwoSize)/
                                     std::log(2.0f)));
  inputBuffer.resize(windowSize * 2, 0.0f);//*see notes at bottom
  windowedInputSegment.resize(windowSize);
  int complexSize = (windowSize/2) + 1;//real and imaginary buffer need half windowsize + 1
  realBuffer.resize(complexSize);
  imaginaryBuffer.resize(complexSize);
  window.resize(windowSize);
  windowedOutput.resize(overlap);
  windowedOutput.clear();
  std::vector<float> temp(windowSize, 0.0f);
  for(int i = 0; i < overlap; i++){
    windowedOutput.push_back(temp);
  }
  calculateWindow();                        
}
//---------------------------------------------------
//user is responsible for not asking for a bin that doesn't exist(which bin shouldn't be > windowSize-1)
void STFT::setBin(int whichBin, std::complex<float> complexInput){
  realBuffer[whichBin] = complexInput.real();
  imaginaryBuffer[whichBin] = complexInput.real();
}
void STFT::setBinMagnitude(int whichBin, float newMagnitude){
  std::complex<float> bin = {realBuffer[whichBin], imaginaryBuffer[whichBin]};
  float magnitude = newMagnitude;
  float phase = std::arg(bin);
  bin = std::polar(magnitude, phase);
  realBuffer[whichBin] = bin.real();
  imaginaryBuffer[whichBin] = bin.imag();
}
void STFT::setBinPhase(int whichBin, float newPhase){
  std::complex<float> bin = {realBuffer[whichBin], imaginaryBuffer[whichBin]};
  float magnitude = std::abs(bin);
  float phase = newPhase;
  bin = std::polar(magnitude, phase);
  realBuffer[whichBin] = bin.real();
  imaginaryBuffer[whichBin] = bin.imag();
}
 
float STFT::getCurrentSample(){return currentSample;}
Window::Mode STFT::getWindowType(){return windowType;}
int STFT::getWindowSize(){return windowSize;}
int STFT::getOverlap(){return overlap;}
int STFT::getHopSize(){return hopSize;}
int STFT::getNumberOfBins(){return windowSize/2;}
std::complex<float> STFT::getBin(int whichBin){//git bin's real and imaginary components
  std::complex<float> bin;//temporary bin 
  //assign from calculated fft
  bin.real(realBuffer[whichBin]);
  bin.imag(imaginaryBuffer[whichBin]);
  return bin;
}
float STFT::getBinMagnitude(int whichBin){
  std::complex<float> bin = {realBuffer[whichBin], imaginaryBuffer[whichBin]};
  return std::abs(bin);
}
float STFT::getBinPhase(int whichBin){
  std::complex<float> bin = {realBuffer[whichBin], imaginaryBuffer[whichBin]};
  return std::arg(bin);
}
float STFT::getBinFrequency(int whichBin){
  float binBandwidth = pdlSettings::sampleRate / static_cast<float>(windowSize);
  float halfBandwidth = binBandwidth * 0.5f;
  return binBandwidth * whichBin + halfBandwidth;
}
float* STFT::getRealBufferPointer(){
  return realBuffer.data();
}
float* STFT::getImaginaryBufferPointer(){
  return imaginaryBuffer.data();
}
//------------------------Private functions
void STFT::calculateWindow(){
  //calculate once since used 'windowSize' times
  float phaseReciprocal = 1.0f/static_cast<float>(windowSize - 1);
  switch(windowType){//fill 'window' vector based on window type
    case Window::Mode::HANNING:
      for(int i = 0; i < windowSize; i++){
        float phase = i * phaseReciprocal;
        window[i] = Window::hanningFromPhase(phase);
      }
    break;
    case Window::Mode::HAMMING:
      for(int i = 0; i < windowSize; i++){
        float phase = i * phaseReciprocal;
        window[i] = Window::hammingFromPhase(phase);
      }
    break;
    case Window::Mode::COSINE:
      for(int i = 0; i < windowSize; i++){
        float phase = i * phaseReciprocal;
        window[i] = Window::cosineFromPhase(phase);
      }
    break;
    case Window::Mode::TRIANGULAR:
      for(int i = 0; i < windowSize; i++){
        float phase = i * phaseReciprocal;
        window[i] = Window::triangularFromPhase(phase);
      }
    break;
    case Window::Mode::BLACKMAN_NUTALL:
      for(int i = 0; i < windowSize; i++){
        float phase = i * phaseReciprocal;
        window[i] = Window::blackmanNutallFromPhase(phase);
      }
    break;
    case Window::Mode::GAUSSIAN:
      for(int i = 0; i < windowSize; i++){
        float phase = i * phaseReciprocal;
        window[i] = Window::gaussianFromPhase(phase);
      }
    break;
  }
}
// calculateWindowedInput() is in header b/c inlined