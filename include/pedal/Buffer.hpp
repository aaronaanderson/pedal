#ifndef Buffer_hpp
#define Buffer_hpp

#include "utilities.hpp"
#include "Interpolation.hpp"
#include "TSine.hpp"
#include <iostream>
#include "../../external/dr_wav.h"//for reading and writing .wav

/*
Buffers are used to store audio. Each buffer may contain
a number of channels, but will contain only one channel by
default. The channels are interleaved. A soundfile may be
read in to a buffer, and a buffer may be saved to disk as
a soundfile. Currently, only .wav is supported.
*/
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
  
  void writeSample(float inputSample, int index, int channel = 0);
  void addToSample(float inputSample, int index, int channel = 0);
  
  void loadSoundFile(const char* soundFilePath);
  void writeSoundFile(const char* destinationPath);

  void setDuration(float newDuration);
  void setDurationInSamples(unsigned long newDurationInSamples);
  
  void fillSineSweep(float lowFrequency = 20.0f, float highFrequency = 20000.0f);
  void fillNoise();
  float getSample(float index, int channel = 0);//interleaved retrieval (can request floating point index)
  float getSample(int index, int channel = 0);//non-interleaved retrieval
  float* getContent();
  float getDuration();
  unsigned long getDurationInSamples();
  int getNumberChannels();
  private:
  float* content = nullptr;//the actual buffer data
  unsigned numberChannels;//number of channels
  long unsigned numberFrames;//could potentially be massive
  char* pathToLoad = nullptr;//storage for path in case of loading
  unsigned fileChannels, fileSampleRate;//This information is needed for proper playback
  drwav_uint64 totalFramesInFile;
  //const drwav_allocation_callbacks callBack;//aaron don't forget about this
  long unsigned durationInSamples;
  long unsigned totalNumberOfSamples;
  float duration;

  drwav_data_format outputFormat;
  drwav wavTemp;
};
/*
TODO make buffer N channel
*/
#endif