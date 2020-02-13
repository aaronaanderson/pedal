#include "pedal/Buffer.hpp"
#include <cstring> // memcpy

//Constructors and deconstructors=====================
// -----rule of 5 --------------------
Buffer::Buffer(float initialDuration){
  setDuration(initialDuration);  
}

Buffer::Buffer(const Buffer& other) {
  duration = other.duration;
  sizeInSamples = other.sizeInSamples;
  content = new float[sizeInSamples];
  std::memcpy(content, other.content, sizeInSamples * sizeof(float));
}

Buffer::Buffer(Buffer&& other) noexcept {
  duration = other.duration;
  sizeInSamples = other.sizeInSamples;
  content = other.content;
  other.content = nullptr;
}

Buffer& Buffer::operator=(const Buffer& other) {
  duration = other.duration;
  sizeInSamples = other.sizeInSamples;
  delete[] content; // delete what we have
  content = new float[sizeInSamples];
  std::memcpy(content, other.content, sizeInSamples * sizeof(float));
  return *this;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
  if (this != &other) { // check self assignment
    duration = other.duration;
    sizeInSamples = other.sizeInSamples;
    delete[] content; // delete what we have
    content = other.content;
    other.content = nullptr;
  }
  return *this;
}
//-----------------------------------
Buffer::~Buffer(){
  delete[] content;
}

//Core functionality of class=========================
void Buffer::writeSample(float inputSample, int index){
  index = clamp(index, 0, sizeInSamples-1);
  content[index] = inputSample;
}

void Buffer::addToSample(float inputSample, int index){
  index = clamp(index, 0, sizeInSamples-1);
  content[index] += inputSample;
}

float Buffer::getSample(float index){
  index = clamp(index, 0.0f, sizeInSamples-1);//clamp for safety
  float retrievedSample = 0.0f;//start with a sample
  //do some linear interpolation
  float previousSample = content[int(index)];// take the 'floor' 
  //grab the next sample. wrap for edge cases
  float nextSample = content[int(index+1.0f)%sizeInSamples];// take the 'ceiling'
  //interpolate between the samples
  retrievedSample = linearInterpolation(index, previousSample, nextSample);
  return retrievedSample;
}

void Buffer::loadSoundFile(char* pathToFile){
    
    
    content = drwav_open_file_and_read_pcm_frames_f32(pathToFile,
                                                       &fileChannels, 
                                                       &fileSampleRate, 
                                                       &totalFramesInFile,
                                                       NULL);
                                                       

  /*
  waveFile = drwav_open_file(pathToFile);
  if (waveFile == nullptr) {
    std::cout << "error loading soundfile" << std::end;
  }else{
    content = (float*)malloc((size_t)waveFile->totalPCMFrameCount *
                              waveFile->channels * sizeof(float));
    drwav_read_f32(waveFile, waveFile->totalPCMFrameCount, content); 
  }

  */
  

}

void Buffer::fillSineSweep(float lowFrequency, float highFrequency){
  float lowExponent = log(lowFrequency)/log(2.0f);//2^x = 20.0Hz, find x
  float highExponent = log(highFrequency)/log(2.0f);//2^x = 20,000Hz, find x
  float linearRange = highExponent - lowExponent;
  TSine sineOscillator;//briefly create a sine oscillator to generate samples
  for(int i = 0; i < sizeInSamples; i++){//for every sample in array
    float linearPosition = (i/float(sizeInSamples))*linearRange;//find the linear position between ranges
    float frequency = pow(2, linearPosition);//determine appropriate frequency for sample
    sineOscillator.setFrequency(frequency);//set oscillator frequency
    content[i] = sineOscillator.generateSample();//generate and assign sample to array
  }
}
void Buffer::fillNoise(){
  for(int i = 0; i < sizeInSamples; i++){
    content[i] = rangedRandom(-1.0f, 1.0f);
  }
}
void Buffer::fillSinc(int numberZeroCrossings){
  for(int i = 0; i < sizeInSamples; i++){
    float currentSample = 0.0f;
    //convert buffer index to phase position (-PI to PI
    float sincPhase = ((i*2)/(float)sizeInSamples) - 1.0f;
    for(float j = 1.0f; j < numberZeroCrossings+1.0f; j += 1.0f){
      currentSample += std::sin(sincPhase*M_PI*j)/(sincPhase);
    }
    content[i] = currentSample;
  }
}

//getters and setters================================
void Buffer::setDuration(float newDuration){
  duration = newDuration;
  delete[] content;
  sizeInSamples = msToSamples(duration);
  content = new float[sizeInSamples];
  for(int i = 0; i < sizeInSamples; i++){
    content[i] = 0.0f;
  }
}

void Buffer::setSizeInSamples(int newSizeInSamples){
    sizeInSamples = newSizeInSamples;
    duration = samplesToMS(sizeInSamples);
}

float Buffer::getDuration(){return duration;}
float* Buffer::getContent(){return content;}
int Buffer::getSizeInSamples(){return sizeInSamples;}