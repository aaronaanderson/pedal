#ifndef utilities_hpp
#define utilities_hpp

#include "math.h"

float mtof(float midiValue){//midi to frequency
    return 440.0f * (pow(2, (midiValue-69)/12.0f));
}

float clamp(float input, float lowerBound, float upperBound){
    return fmin(1.0, fmax(input, 0.0));
}
#endif