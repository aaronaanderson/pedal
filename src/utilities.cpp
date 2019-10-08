#include "pedal/utilities.hpp"

float mtof(float midiValue){//midi to frequency
    return 440.0f * (pow(2, (midiValue-69)/12.0f));
}

float clamp(float input, float lowerBound, float upperBound){
    return fmin(1.0, fmax(input, 0.0));
}

float rangedRandom(float minimum, float maximum){
  float signedHalfNormal = (rand()/float(RAND_MAX))-0.5f;
  float range = maximum - minimum;
  float center = (maximum + minimum) * 0.5f;
  return (signedHalfNormal*range) + center;
}

void normalizeBuffer(float* inputBuffer, int bufferSize, bool correctDC = true){
  float highestValue;
  if(correctDC){
    float lowestValue = 100000.0f;//TODO make inf
    highestValue = -100000.0f;//TOOD make -inf
    for(int i = 0; i < bufferSize; i++){
      lowestValue = fmin(inputBuffer[i], lowestValue);
      highestValue = fmax(inputBuffer[i], highestValue);
    }
    float currentCenter = (highestValue - lowestValue) * 0.5;//find the average
    //offsetValue is added to each sample to bring to 0.0
    float offsetValue = -currentCenter;
    //adjust all values to have a currentCenter of 0.0
    for(int i = 0; i < bufferSize; i++){//for each sample in the buffer
      inputBuffer[i] += offsetValue;//addjust uniformly
    }
  }
  if(!correctDC){//scan to find the highest value since that hasn't been done
    highestValue = 0.0f;//TODO this should really be -inf
    //find the largest value in the buffer
    for(int i = 0; i < bufferSize; i++){//for every sample in the buffer
      highestValue = fmax(fabs(inputBuffer[i]), highestValue);
      //the absolute value is taken as the value closest to the boundry,
      //-1 to 1, could be negative or positive, but we don't need the sign, 
      //just 'how close is the closest sample to the boarder?'
    }
  }
  //Do some math, largestValue*scalarValue=1.0
  float scalarValue = 1/highestValue;
  //scale the entire buffer uniformly by this scalar value
  for(int i = 0; i < bufferSize; i++){
    inputBuffer[i] *= scalarValue;
  }
}

