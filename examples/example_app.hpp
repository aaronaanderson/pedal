#ifndef PDL_EX_APP
#define PDL_EX_APP

struct pdlExampleApp;
using pdlExampleCallback = void (*)(float* out, float* in, unsigned bufferSize,
                                    unsigned samplingRate, unsigned numChannelsOut,
                                    unsigned numChannelsIn,double streamTime, 
                                    pdlExampleApp* app);

pdlExampleApp* pdlInitExampleApp(pdlExampleCallback callback);
void pdlStartExampleApp(pdlExampleApp* app);
bool pdlRunExampleApp(pdlExampleApp* app);
void pdlUpdateExampleApp(pdlExampleApp* app);
void pdlDeleteExampleApp(pdlExampleApp* app);
unsigned pdlExampleAppGetSamplingRate(pdlExampleApp* app);
unsigned pdlExampleAppGetBufferSize(pdlExampleApp* app);

void pdlGetCursorPos(pdlExampleApp* app, float* mx, float* my);

void pdlAddSlider(pdlExampleApp* app, int sliderIndex, const char* name,
                  float low, float high, float initialValue);
float pdlGetSlider(pdlExampleApp* app, int idx);

void pdlAddToggle(pdlExampleApp* app, int toggleIndex, const char* name,
                  bool initialValue);
bool pdlGetToggle(pdlExampleApp* app, int idx);

//void pdlAddDropdown(pdlExampleApp* app, )
void pdlAddTrigger(pdlExampleApp* app, int triggerIndex, const char* name);
bool pdlGetTrigger(pdlExampleApp* app, int idx);

void pdlAddDropDown(pdlExampleApp* app, int idx, const char* name,  char*  content[], int length);
int pdlGetDropDown(pdlExampleApp* app, int indx);
#endif
