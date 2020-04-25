#ifndef Counter_hpp
#define Counter_hpp

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
#endif 