#ifndef BufferPlayer_hpp
#define BufferPlayer_hpp

#include "Buffer.hpp"
#include "utilities.hpp"

enum InterpolationMode{
  NONE = 0, //may be noisey if speed != integer multiple of 1
  LINEAR, //lower cost, but great results
  CUBIC //expensive but best
};

enum PlayMode{
  ONE_SHOT = 0, //play to end then stop
  LOOP, //play to end than move to other end immediately
  PING_PONG //play to end, then reverse directions
  //TODO LOOP_DUCKING //loop with auto-cross-faded ends
};

class BufferPlayer{
  public:
  BufferPlayer(Buffer* reference = nullptr);//construct the player (with reference, if provided)
  float update();//progress (and calculate if needed) new samples
  
  void play();//make 'isPlaying' true
  void pause();//make 'isPlaying' false
  void stop();//make 'isPlaying' false, return to beginning
  void reverseDirection();//move from forwards->backwards, or vice versa
  void setSpeed(float newSpeed);//change speed (1.0f is normal playback)
  void setPlayMode(PlayMode newPlayMode);//change mode
  void setReference(Buffer* newReference);//assign new buffer
  void setInterpolationMode(InterpolationMode newMode);//set interpolation mode
  float getSample(int channel = 0);//get a single sample
  float* getFrame();//get pointer to frame of samples

  private:
  PlayMode playMode; //internal storage of edge handling mode
  InterpolationMode interpolationMode;//how are inbetween samples calculated?
  float* currentFrame = nullptr; //pointer to array of samples, size numberChannels
  void assignDataFromReference(Buffer* reference);//reuseable function
  int wrapIndex(int inputIndex);//needed often for interpolation
  float index;//floating point, since playback can be between integer samples
  unsigned totalSampleCount;//total number of samples in reference
  int numberChannels;//adapts automatically from buffer reference
  float playSpeed;//playback speed. Can be negative.
  float direction;//direction storage (used to flip direction)
  bool isPlaying;//condition set by play/pause/stop functions
  Buffer* bufferReference;//a pointer to a buffer class
};
#endif 