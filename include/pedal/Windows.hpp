#ifndef windows_hpp
#define windows_hpp

class HanningWindow{
  public:
  HanningWindow();
  ~HanningWindow();

  float generateSample();
  float* generateBlock();  
  private:
  float currentSample;
  float* currentBlock;
};
#endif