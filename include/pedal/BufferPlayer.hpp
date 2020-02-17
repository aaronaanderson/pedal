#ifndef BufferPlayer_hpp
#define BufferPlayer_hpp

#include "Buffer.hpp"
#include "utilities.hpp"

enum InterpolationMode{
  NONE = 0, 
  LINEAR, 
  CUBIC
};

enum PlayMode{
  ONE_SHOT = 0, 
  LOOP, 
  PING_PONG
  //TODO LOOP_DUCKING 
};

class BufferPlayer{
  public:
  BufferPlayer(Buffer* reference = nullptr);
  float update();
  
  void play();
  void pause();
  void stop();
  
  void reverseDirection();
  void setSpeed(float newSpeed);
  void setPlayMode(PlayMode newPlayMode);

  float getSample(int channel = 0);
  float* getFrame();

  private:
  PlayMode playMode;
  InterpolationMode interpolationMode;
  float* currentFrame = nullptr;
  void assignDataFromReference(Buffer* reference);
  int wrapIndex(int inputIndex);
  float index;
  unsigned totalSampleCount;
  int numberChannels;
  float playSpeed;
  bool isPlaying;
  Buffer* bufferReference;
};
#endif 