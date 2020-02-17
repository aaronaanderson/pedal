#ifndef BufferPlayer_hpp
#define BufferPlayer_hpp

#include "Buffer.hpp"

enum interpolation{
  NONE = 0, 
  LINEAR, 
  CUBIC
};

enum playMode{
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
  float* currentFrame = nullptr;
  void assignDataFromReference(Buffer* reference);
  float index;
  unsigned totalSampleCount;
  int numberChannels;
  float playSpeed;
  Buffer* bufferReference;
};
#endif 