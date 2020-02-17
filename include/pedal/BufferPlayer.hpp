#ifndef BufferPlayer_hpp
#define BufferPlayer_hpp

#include "Buffer.hpp"

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
  
  
  void setSpeed(float newSpeed);
  float getSample(int channel = 0);
  float* getFrame();

  private:
  PlayMode playMode;
  InterpolationMode interpolationMode;
  float* currentFrame = nullptr;
  void assignDataFromReference(Buffer* reference);
  float index;
  unsigned totalSampleCount;
  int numberChannels;
  float playSpeed;
  Buffer* bufferReference;
};
#endif 