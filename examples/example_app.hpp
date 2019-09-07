#ifndef PDL_EX_APP
#define PDL_EX_APP

struct pdlExampleApp;
using pdlExampleCallback = void (*)(float* out, unsigned bufferSize,
                                    unsigned samplingRate, unsigned numChannels,
                                    double streamTime, pdlExampleApp* app);

pdlExampleApp* pdlInitExampleApp(pdlExampleCallback callback);
void pdlStartExampleApp(pdlExampleApp* app);
bool pdlRunExampleApp(pdlExampleApp* app);
void pdlUpdateExampleApp(pdlExampleApp* app);
void pdlDeleteExampleApp(pdlExampleApp* app);

void pdlAddSlider(pdlExampleApp* app, int sliderIndex, const char* name,
                  float low, float high, float initialValue);
float pdlGetSlider(pdlExampleApp* app, int idx);

#endif
