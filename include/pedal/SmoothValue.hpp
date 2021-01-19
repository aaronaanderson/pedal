 /*
This class is a low pass filter for control
input. Typically control rate is lower than
audio rate. Direct use of control data results
in sudden change in the audio data which creates
undesired sounds.
*/
#ifndef SmoothValue_hpp
#define SmoothValue_hpp

namespace pedal{

template <class T>  //The class is templated to allow a variety of data types
class SmoothValue{
  public:
  SmoothValue(float initialTime = 50.0f){//how much time, in ms, does it take to arrive (or approach target value)
    arrivalTime = initialTime;//store the input value (default to 50ms)
    calculateCoefficients();//calculate a and b (it is a lowpass filter)
    z = 0.0f;
  }
  inline T process(){//this function will be called per-sample on the data
    z = (targetValue * b) + (z * a);//when evaluated, z is the 'previous z' (it is leftover from last execution)
    return z;//return the new z value (the output sample)
  }
  void setTime(float newTime){//set time (in ms)
    arrivalTime = newTime;//store the input value
    calculateCoefficients();//calculate a and b
  }
  void setTarget(T newTarget){targetValue = newTarget;}
  T getCurrentValue(){return z;}
  T getTargetValue(){return targetValue;}
  float getTime(){return arrivalTime;}

  private:
  void calculateCoefficients(){//called only when 'setTime' is called (and in constructor)
    a = std::exp(-(pedal::TWOPI) / (arrivalTime * 0.001f * Settings::sampleRate));//rearranged lpf coeff calculations
    b = 1.0f - a;
  }
  T targetValue;//what is the destination (of type T, determind by implementation)
  T currentValue;//how close to the destination? (output value)
  float arrivalTime;//how long to take
  float a,b;//coefficients
  T z;//storage for previous value
};
}//end namespace pedal
#endif