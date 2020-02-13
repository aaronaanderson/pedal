#ifndef Buffer_hpp
#define Buffer_hpp

#include "utilities.hpp"
#include "Interpolation.hpp"
#include "TSine.hpp"
#include <iostream>
#include "../../external/dr_wav.h"//for reading and writing .wav

class Buffer{
  public:
  Buffer(float initialDuration = 1000.0f);

  // mandatory video for cpp programmers!
  // rule of 0, 3, and 5, copy contructors, move constructors,
  // copy assignment operators, and move assignment operators
  // https://youtu.be/7Qgd9B1KuMQ

  // copy ctor
  // ex) Buffer b1 = b0;
  Buffer(const Buffer& other);

  // move dtor
  // ex) Buffer b1 = functionThatReturnsBuffer();
  // noexcept: tell compiler that no exception will happen so it can optimize
  Buffer(Buffer&& other) noexcept;

  // copy assignment
  // ex) Buffer b1;
  //     b1 = b0;
  Buffer& operator=(const Buffer& other);
  
  // move assignment
  // ex) Buffer b1;
  // b1 = functionThatReturnsBuffer();
  Buffer& operator=(Buffer&& other) noexcept;

  // deconstructor
  ~Buffer();
  
  void writeSample(float inputSample, int index);
  void addToSample(float inputSample, int index);
  
  void loadSoundFile(char* soundFilePath);
  void writeSoundFile(char* destinationPath);

  void setDuration(float newDuration);
  void setSizeInSamples(int newSizeInSamples);
  
  void fillSineSweep(float lowFrequency = 20.0f, float highFrequency = 20000.0f);
  void fillNoise();
  void fillSinc(int numberZeroCrossings = 20);
  float getSample(float index);
  float* getContent();
  float getDuration();
  int getSizeInSamples();
  
  private:
  drwav* waveFile;
  float* content = nullptr;
  char* pathToLoad = nullptr;//storage for path in case of loading
  unsigned fileChannels, fileSampleRate;
  drwav_uint64 totalFramesInFile;
  //const drwav_allocation_callbacks callBack;//aaron don't forget about this
  int sizeInSamples;
  float duration;
};
/*
TODO make buffer N channel
*/
#endif