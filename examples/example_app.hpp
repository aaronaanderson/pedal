/*
// Minimal functionality to build simple examples
*/
#ifndef PDL_EX_APP
#define PDL_EX_APP

struct PedalExampleApp;
using pdlExampleAudioCallback = void (*)(float* out, float* in, int bufferSize,
                                         int numChannelsOut, int numChannelsIn, 
                                         PedalExampleApp* app);
#include <vector>
using pdlExampleMidiInputCallback = void (*)(double deltatime, 
                                             std::vector< unsigned char >* message,
                                             PedalExampleApp* app);

PedalExampleApp* pdlInitializeExampleApp(pdlExampleAudioCallback, int sampleRate = 48000, int bufferSize = 512);

void pdlOpenMidiPort(PedalExampleApp* app, int port);
void pdlStartExampleApp(PedalExampleApp* app);
bool pdlRunExampleApp(PedalExampleApp* app);
void pdlUpdateExampleApp(PedalExampleApp* app);
void pdlDeleteExampleApp(PedalExampleApp* app);

void pdlSetMidiCallback(PedalExampleApp* app, pdlExampleMidiInputCallback);
void pdlSetKeyboardCallback(void (*)(int keyPressed, bool keyDown));

unsigned pdlGetSampleRate(PedalExampleApp* app);
unsigned pdlGetBufferSize(PedalExampleApp* app);

void pdlGetCursorPos(PedalExampleApp* app, float* mx, float* my);

void pdlAddSlider(PedalExampleApp* app, int sliderIndex, const char* name,
                  float low, float high, float initialValue);
float pdlGetSlider(PedalExampleApp* app, int idx);

void pdlAddToggle(PedalExampleApp* app, int toggleIndex, const char* name,
                  bool initialValue);
bool pdlGetToggle(PedalExampleApp* app, int idx);

//void pdlAddDropdown(PedalExampleApp* app, )
void pdlAddTrigger(PedalExampleApp* app, int triggerIndex, const char* name);
bool pdlGetTrigger(PedalExampleApp* app, int idx);

void pdlAddDropDown(PedalExampleApp* app, int idx, const char* name,  
                    char*  content[], int length);
int pdlGetDropDown(PedalExampleApp* app, int indx);

int pdlAsciiToMidi(int ascii);

#include <string>
std::string pdlGetPathToSoundFiles();

#endif
