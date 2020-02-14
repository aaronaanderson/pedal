#include "pedal/Buffer.hpp"
#include <cstring> // memcpy

//Constructors and deconstructors=====================
// -----rule of 5 --------------------
Buffer::Buffer(float initialDuration){
  setDuration(initialDuration);  
  format.container = drwav_container_riff;// <-- drwav_container_riff = normal WAV files, drwav_container_w64 = Sony Wave64.
  format.format = DR_WAVE_FORMAT_PCM; // <-- Any of the DR_WAVE_FORMAT_* codes.
  format.channels = 1;
  format.sampleRate = pdlSettings::sampleRate;
  format.bitsPerSample = 32;
}

Buffer::Buffer(const Buffer& other) {
  duration = other.duration;
  durationInSamples = other.durationInSamples;
  content = new float[durationInSamples];
  std::memcpy(content, other.content, durationInSamples * sizeof(float));
}

Buffer::Buffer(Buffer&& other) noexcept {
  duration = other.duration;
  durationInSamples = other.durationInSamples;
  content = other.content;
  other.content = nullptr;
}

Buffer& Buffer::operator=(const Buffer& other) {
  duration = other.duration;
  durationInSamples = other.durationInSamples;
  delete[] content; // delete what we have
  content = new float[durationInSamples];
  std::memcpy(content, other.content, durationInSamples * sizeof(float));
  return *this;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
  if (this != &other) { // check self assignment
    duration = other.duration;
    durationInSamples = other.durationInSamples;
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
  index = clamp(index, 0, durationInSamples-1);
  content[index] = inputSample;
}

void Buffer::addToSample(float inputSample, int index){
  index = clamp(index, 0, durationInSamples-1);
  content[index] += inputSample;
}

float Buffer::getSample(float index){
  index = clamp(index, 0.0f, durationInSamples-1);//clamp for safety
  float retrievedSample = 0.0f;//start with a sample
  //do some linear interpolation
  float previousSample = content[int(index)];// take the 'floor' 
  //grab the next sample. wrap for edge cases
  float nextSample = content[int(index+1.0f)%durationInSamples];// take the 'ceiling'
  //interpolate between the samples
  retrievedSample = linearInterpolation(index, previousSample, nextSample);
  return retrievedSample;
}
float Buffer::getSample(int index){
  index = clamp(index, 0.0f, durationInSamples-1);//clamp for safety
  return content[index];
}

void Buffer::loadSoundFile(const char* pathToFile){
    float* temporaryPointer;
    temporaryPointer = drwav_open_file_and_read_pcm_frames_f32(pathToFile,
                                                       &fileChannels, 
                                                       &fileSampleRate, 
                                                       &totalFramesInFile,
                                                       NULL);
  if (temporaryPointer == nullptr) {//if the loading failed
    std::cout << "error loading soundfile" << std::endl;
  }else{//if the file successfully loaded
  //TODO adjust for sampling rate differences
    //how many samples long is the buffer?
    durationInSamples = totalFramesInFile/fileChannels;
    setDurationInSamples(durationInSamples);
    content = temporaryPointer; 
  }
}

void Buffer::writeSoundFile(const char* pathToFile){
  drwav_init_file_write_sequential_pcm_frames(&wavTemp, "data/recording.wav", &format, durationInSamples, NULL);

  drwav_uint64 framesWritten = drwav_write_pcm_frames(&wavTemp, 
                                                     durationInSamples,//how many frames 
                                                     content );
  std::cout << framesWritten/pdlSettings::sampleRate << std::endl;
}

void Buffer::fillSineSweep(float lowFrequency, float highFrequency){
  float lowExponent = log(lowFrequency)/log(2.0f);//2^x = 20.0Hz, find x
  float highExponent = log(highFrequency)/log(2.0f);//2^x = 20,000Hz, find x
  float linearRange = highExponent - lowExponent;
  TSine sineOscillator;//briefly create a sine oscillator to generate samples
  for(int i = 0; i < durationInSamples; i++){//for every sample in array
    float linearPosition = (i/float(durationInSamples))*linearRange;//find the linear position between ranges
    float frequency = pow(2, linearPosition);//determine appropriate frequency for sample
    sineOscillator.setFrequency(frequency);//set oscillator frequency
    content[i] = sineOscillator.generateSample();//generate and assign sample to array
  }
}
void Buffer::fillNoise(){
  for(int i = 0; i < durationInSamples; i++){
    content[i] = rangedRandom(-1.0f, 1.0f);
  }
}
//getters and setters================================
void Buffer::setDuration(float newDuration){
  duration = newDuration;
  delete[] content;
  durationInSamples = msToSamples(duration);
  content = new float[durationInSamples];
  for(int i = 0; i < durationInSamples; i++){
    content[i] = 0.0f;
  }
}
void Buffer::setDurationInSamples(unsigned long newdurationInSamples){
    durationInSamples = newdurationInSamples;
    duration = samplesToMS(durationInSamples);
    delete[] content;
    content = new float[durationInSamples];
    for(int i = 0; i < durationInSamples; i++){
      content[i] = 0.0f;
    }
}

float Buffer::getDuration(){return duration;}
float* Buffer::getContent(){return content;}
unsigned long Buffer::getDurationInSamples(){return durationInSamples;}