#include "pedal/utilities.hpp"

void normalize(float* inputBuffer, int bufferSize){
  float largestValue = 0.0f;//TODO this should really be -inf
  //find the largest value in the buffer
  for(int i = 0; i < bufferSize; i++){
    largestValue = fmax(fabs(inputBuffer[i]), largestValue);
  }
  //largestValue*scalarValue=1.0
  float scalarValue = 1/largestValue;
  //scale the entire buffer uniformly by this scalar value
  for(int i = 0; i < bufferSize; i++){
    inputBuffer[i] *= scalarValue;
  }
}