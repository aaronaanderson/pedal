#include "pedal/Interpolation.hpp"


float cubicInterpolation(float inputSample, float backTwo, float backOne, 
                         float forwardOne, float forwardTwo){
    float x = inputSample - int(inputSample);//position between samples
    float a = -0.5*backTwo + 1.5*backOne - 1.5*forwardOne + 0.5 * forwardTwo;
    float b = backTwo - 2.5*backOne + 2*forwardOne - 0.5*forwardTwo;
    float c = -0.5*backTwo + 0.5*forwardOne;
    float d = backOne;
    return (a*pow(x, 3)) + (b*pow(x, 2)) + (c*x) + d;
};

