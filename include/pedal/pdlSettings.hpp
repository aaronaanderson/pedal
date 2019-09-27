#ifndef pdlSettings_hpp
#define pdlSettings_hpp

class pdlSettings{
  public:
    static double sampleRate;
    static int bufferSize;
    void setup(int sampleRate, int bufferSize){
        pdlSettings::sampleRate = sampleRate;
        pdlSettings::bufferSize = bufferSize;
    }

  private:
    pdlSettings(){};

};

#endif 