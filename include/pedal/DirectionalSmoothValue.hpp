/*

*/
#ifndef DirectionalSmoothValue_hpp
#define DirectionalSmoothValue_hpp

namespace pedal{

template <class T>
class DirectionalSmoothValue{
public:
  DirectionalSmoothValue(float initialTimeUp = 50.0f, float initialTimeDown = 50.0f){//how much time, in ms, does it take to arrive (or approach target value)
    arrivalTimeUp = initialTimeUp;//store the input value (default to 50ms)
    calculateUpCoefficients();//calculate aUp and bUp
    arrivalTimeDown = initialTimeDown;//store input value (default 50ms)
    calculateDownCoefficients();//calculate aDown and bDown
    z = 0.0f;//zero the 'previous' sample
    up = true;//set a default value, will be overwritten on first target value if needed
  }
  inline T process(){//this function will be called per-sample on the data
    if(up){
      //when evaluated, z is the 'previous z' (it is leftover from last execution)
      z = (targetValue * bUp) + (z * aUp);
    }else{//if going down/decending
      z = (targetValue * bDown) + (z * aDown);
    }
    return z;//return the new z value (the output sample)
  }
  void setTimeUp(float newTimeUp){//set time Up (in ms)
    arrivalTimeUp = newTimeUp;//store the input value
    calculateUpCoefficients();//calculate aUp and bUp
  }
  void setTimeDown(float newTimeDown){//set time down (in ms)
    arrivalTimeDown = newTimeDown;//store the new value
    calculateDownCoefficients();//calculate aDown and bDown
  }
  void setTarget(T newTarget){//set a target value
    if(newTarget > z){//if target is greater than current value
      up = true;//then direction is up
    }else{//if target is <= current value
      up = false;//direction is down
    }
    targetValue = newTarget;//store new target
  }
  T getCurrentValue(){return z;}
  T getTargetValue(){return targetValue;} 
  float getTimeUp(){return arrivalTimeUp;}
  float getTimeDown(){return arrivalTimeDown;}

  private:
  void calculateUpCoefficients(){//called only when 'setTimeUp' is called (and in constructor)
    if(arrivalTimeUp > 0.0f){//avoid / 0
      aUp = std::exp(-(pedal::TWOPI) / (arrivalTimeUp * 0.001f * Settings::sampleRate));//rearranged lpf coeff calculations
      bUp = 1.0f - aUp;
    }else{
      aUp = 0.0f;
      bUp = 1.0f;
    }
  }
  void calculateDownCoefficients(){//called only when 'setTimeUp' is called (and in constructor)
    if(arrivalTimeDown > 0.0f){//avoid / 0
      aDown = std::exp(-(pedal::TWOPI) / (arrivalTimeDown * 0.001f * Settings::sampleRate));
      bDown = 1.0f - aDown;
    }else{
      aDown = 0.0f;
      bDown = 1.0f;
    }
  }
  T targetValue;//what is the destination (of type T, determind by implementation)
  float arrivalTimeUp;//how long to take
  float arrivalTimeDown;
  bool up;//direction for storage: is value going up?
  float aUp,bUp;//coefficients if going up
  float aDown, bDown;//coefficients if going down
  T z;//storage for previous value
};
}
#endif