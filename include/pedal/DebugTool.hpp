/*
A debugging tool specifically useful for audio 
thread debugging
*/
#ifndef DebugTool_hpp
#define DebugTool_hpp

#include <iostream>
#include "pdlSettings.hpp"

class DebugTool{
  public:
  //DebugTool();
  //~DebugTool();
  //void update(int currentFrame);//update the frame flags
  template <class T>
  static void printOncePerBuffer(T whatToPrint, int currentFrame){
    if(currentFrame == 0){//if first frame of buffer
      std::cout << whatToPrint << std::endl;
    }
  }
  template <class T>
  void printOncePerBuffer(T whatToPrint){
    frameCount = frameCount % pdlSettings::bufferSize;
    if(frameCount == 0){
      std::cout << whatToPrint << std::endl;
    }
  }
  //print every Nth buffer
  //void logBuffer(int buffersToSkip)

  bool firstFrameFlag;//has a new buffer started calculating?
  bool lastFrameFlag;//has a buffer just finished calculating?
  private:
  int frameCount = 0;
};
#endif