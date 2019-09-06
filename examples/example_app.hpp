#ifndef PEDAL_EXAMPLE_APP
#define PEDAL_EXAMPLE_APP

struct pdlExampleApp;
using pdlExampleCallback = void (*)(float* out, unsigned bufferSize,
                                    unsigned samplingRate, unsigned numChannels,
                                    double streamTime);

pdlExampleApp* pdlInitExampleApp(pdlExampleCallback callback);
void pdlStartExampleApp(pdlExampleApp* app);
bool pdlRunExampleApp(pdlExampleApp* app);
void pdlUpdateExampleApp(pdlExampleApp* app);
void pdlDeleteExampleApp(pdlExampleApp* app);

#endif
