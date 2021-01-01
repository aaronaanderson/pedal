#include "pedal/Counter.hpp"

using namespace pedal;

Counter::Counter(int countSize){
  counterSize = countSize;
  set(0);
}
int Counter::increment(){
  currentCount++;
  currentCount %= counterSize;
  return currentCount;
}
int Counter::decrement(){
  currentCount--;
  if(currentCount < 0){
    currentCount += counterSize;
  }
  return currentCount;
}
void Counter::reset(){currentCount = 0;}
void Counter::set(int newCount){currentCount = newCount;}
int Counter::getCount(){return currentCount;}