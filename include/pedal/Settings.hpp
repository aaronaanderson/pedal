#ifndef Settings_hpp
#define Settings_hpp

namespace pedal{
class Settings{
  public:
    static double sampleRate;
    static int bufferSize;
    void setup(int sampleRate, int bufferSize){
        Settings::sampleRate = sampleRate;
        Settings::bufferSize = bufferSize;
    }

  private:
    Settings(){};

};
}//end pedal namespace
#endif 