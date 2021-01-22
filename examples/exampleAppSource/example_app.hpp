/*
// Minimal functionality to build simple examples
*/
#ifndef PDL_EX_APP
#define PDL_EX_APP

#include <string>
#include <vector>
namespace app{

struct PedalExampleApp;
using pdlExampleAudioCallback = void (*)(float* out, float* in, int bufferSize,
                                         int numChannelsOut, int numChannelsIn, 
                                         PedalExampleApp* app);

using pdlExampleMidiInputCallback = void (*)(double deltatime, 
                                             std::vector< unsigned char >* message,
                                             PedalExampleApp* app);

PedalExampleApp* initializeExampleApp(pdlExampleAudioCallback, int sampleRate = 48000, int bufferSize = 512);

void openMidiPort(PedalExampleApp* app, int port);
void startApp(PedalExampleApp* app);
bool shouldContinue(PedalExampleApp* app);
void update(PedalExampleApp* app);
void freeMemory(PedalExampleApp* app);

void setMidiCallback(PedalExampleApp* app, pdlExampleMidiInputCallback);
void setKeyboardCallback(void (*)(int keyPressed, bool keyDown));

unsigned getSampleRate(PedalExampleApp* app);
unsigned getBufferSize(PedalExampleApp* app);

void getCursorPos(PedalExampleApp* app, float* mx, float* my);

void addSlider(PedalExampleApp* app, int sliderIndex, const char* name,
                  float low, float high, float initialValue);
float getSlider(PedalExampleApp* app, int idx);

void addToggle(PedalExampleApp* app, int toggleIndex, const char* name,
                  bool initialValue);
bool getToggle(PedalExampleApp* app, int idx);

//void addDropDown(PedalExampleApp* app, )
void addTrigger(PedalExampleApp* app, int triggerIndex, const char* name);
bool getTrigger(PedalExampleApp* app, int idx);

void addDropDown(PedalExampleApp* app, int idx, const char* name,  
                    char*  content[], int length);
int getDropDown(PedalExampleApp* app, int indx);

int asciiToMidi(int ascii);


std::string getPathToSoundFiles();
}//end namespace app
#endif
