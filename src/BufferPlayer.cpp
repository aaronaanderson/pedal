#include "pedal/BufferPlayer.hpp"

//Constructors and deconstructors=====================
BufferPlayer::BufferPlayer(Buffer* reference){//constructor (default)
  if(reference != nullptr){//if the reference isn't invalid (it is by default)
    bufferReference = reference;//assign the input as the reference
    assignDataFromReference(reference);//extract and assign data from this reference
  }
  //assign defaults
  index = 0.0f;//beginning of sound
  playSpeed = 1.0f;//natural playback speed
  playMode = LOOP;//One shot, Loop, or PingPong
  isPlaying = true;//on by default
  interpolationMode = LINEAR;//most common interpolation mode
  direction = 1.0f;//forward -1.0f is backward
}

//Core functionality of class=========================
float BufferPlayer::update(){//function called per-sample
  if(bufferReference != nullptr){//if a buffer reference exists (can't play nothing)
    if(isPlaying){//if not paused/stopped
      for(int i = 0; i < numberChannels; i++){//for every audio channel in the buffer
        //calculate the next sample. This depends on the desired interpolation mode
        switch(interpolationMode){
          case NONE://grab the 'floor' of the index (ignore decimal portion), pull out a sample
            currentFrame[i] = bufferReference->getSample((int)index, i);
          break;
          case LINEAR://Look at value before, and value ahead. choose value based on this
          { //*this process is explained in more detail at the bottom of this document
            float previousSample = bufferReference->getSample((int)index, i);
            float nextSample = bufferReference->getSample(wrapIndex((int)index + numberChannels), i);
            currentFrame[i] = linearInterpolation(index, previousSample, nextSample);
          }
          break;
          case CUBIC://look two samples back and two samples forward to determine the value
          {//*this process is explained in more detail at the bottom of this document
            float backTwo = bufferReference->getSample(wrapIndex((int)index - numberChannels), i);
            float backOne = bufferReference->getSample((int)index, i);
            float forwardOne = bufferReference->getSample(wrapIndex((int)index + numberChannels), i);
            float forwardTwo = bufferReference->getSample(wrapIndex((int)index + numberChannels * 2), i);
            currentFrame[i] = cubicInterpolation(index,backTwo,backOne,forwardOne,forwardTwo);
          }
          break;
        }
      }
      //after the sample is acquired, update the playback position(index)
      index += playSpeed * direction;//Increment by speed (if speed is negative, this is a decrement)
      switch (playMode){
        case ONE_SHOT:
        if(index > bufferReference->getDurationInSamples() || index < 0.0f){
          stop();
        }
        break;
        case LOOP://default
        if(index > bufferReference->getDurationInSamples()){//if index has passed the end
          index -= bufferReference->getDurationInSamples();//subtract the size of the buffer
        }else if(index < 0.0f){//if index has gone past the beginning (possible for negative speeds)
          index += bufferReference->getDurationInSamples();//add the duration to the index (go to end)
        }
        break;
        case PING_PONG://If reached either end, reverse direction and proceed
        if(index > bufferReference->getDurationInSamples()){//if index has over shot
          //mirror position
          //bounce back from the end the same distance it overshot
          float overShoot = index - (float)bufferReference->getDurationInSamples();//how much did it overshoot?
          index -= overShoot*2.0f;//subtract in place
          reverseDirection();
        }
        if(index < 0.0f){//if index has reached beginning
          float overShoot = fabs(index);// how far past the beginning did it go
          index = overShoot;//mirror that back into the buffer
          reverseDirection();
        }
        break;
      }//end of playmode switch statement
    }//end of 'isPlaying' condition
  }//end of nullptr reference check
  return currentFrame[0];
}//end of fucntion
void BufferPlayer::assignDataFromReference(Buffer* reference){
  numberChannels = reference->getNumberChannels();
  delete[] currentFrame;//free previous frame memory
  currentFrame = new float[numberChannels];//allocate new memory
  totalSampleCount = reference->getDurationInSamples() * //sample length of one channel
                     reference->getNumberChannels();
}
int BufferPlayer::wrapIndex(int inputIndex){
  return (inputIndex + bufferReference->getDurationInSamples()) % 
          bufferReference->getDurationInSamples();
}
void BufferPlayer::stop(){
  isPlaying = false;
  index = 0.0f;
}
void BufferPlayer::play(){
  isPlaying = true;
}
void BufferPlayer::pause(){
  isPlaying = false;
}
void BufferPlayer::reverseDirection(){
  direction *= -1.0f;
}
//getters and setters================================
float* BufferPlayer::getFrame(){return currentFrame;}//return a pointer to the frame array
float BufferPlayer::getSample(int channel){//returns channel 0 if bad request
  if(channel < numberChannels){//is this a good request?
    return currentFrame[channel];
  }else{//if not available, send the first channel //TODO is this a bad idea?
    return currentFrame[0];
  }
}
void BufferPlayer::setSpeed(float newSpeed){playSpeed = newSpeed;}
void BufferPlayer::setPlayMode(PlayMode newPlayMode){
  playMode = newPlayMode;
  direction = 1.0f;//start forward, always. (this is only needed because of ping_pong mode)  
}
void BufferPlayer::setReference(Buffer* newReference){bufferReference = newReference;}
void BufferPlayer::setInterpolationMode(InterpolationMode newMode){interpolationMode = newMode;}

//=============further explenation
/*

For interpolation, it is necessary to look back and forward from the current index. 
If the index is 60.3f, we are interested in samples 60 and 61 to determine a value. 
Because 60.3 is closer to 60 than it is to 61, expect the value at 60 to have more 
influence on the calculated value.

To find the indices, simply cast the index as an int to find the previous entry, and
add one to this value to find the next. In this class, however, the buffer is interlieved.
The sample stored immediately after the sample we are interested may belong to a different
channel if there is more than one channel.Instead of adding 1, numberChannels is added.

Because it was needed often, I created a wrapIndex(int input) function. This function 
checks the index against the size of the array, and wrapps it back around if necessary.

Finally, the getSample(index, channel) can take a channel value. We conveniently use our
position in the for loop, i, for this.

float prevoiusSample = bufferReference->getSample((int)index, i);
float nextSample = bufferReference->getSample(wrapIndex((int)index + numberChannels), i);
currentFrame[i] = linearInterpolation(index, previousSample, nextSample);

In cubic interpolation, the story is very similar. The main difference is that this 
type of interpolation requires two entries behind the index and two entries in front of 
the index.

float backTwo = bufferReference->getSample(wrapIndex((int)index - numberChannels), i);
float backOne = bufferReference->getSample((int)index, i);
float forwardOne = bufferReference->getSample(wrapIndex((int)index + numberChannels), i);
float forwardTwo = bufferReference->getSample(wrapIndex((int)index + numberChannels * 2), i);
currentFrame[i] = cubicInterpolation(index,backTwo,backOne,forwardOne,forwardTwo);

*/