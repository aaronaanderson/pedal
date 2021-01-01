#ifndef Counter_hpp
#define Counter_hpp

namespace pedal{
class Counter{
  public:
  Counter(int counterSize = 8);
  int increment();
  int decrement();
  void reset();
  void set(int newCount);
  int getCount();
  
  private:
  int currentCount;
  int counterSize;
};
}//end pedal namespace
#endif 