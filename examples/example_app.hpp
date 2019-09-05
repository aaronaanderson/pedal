#ifndef PEDAL_EXAMPLE_APP
#define PEDAL_EXAMPLE_APP

struct pdlExampleApp;

pdlExampleApp* pdlInitExampleApp();
bool pdlRunExampleApp(pdlExampleApp* app);
void pdlUpdateExampleApp(pdlExampleApp* app);
void pdlDeleteExampleApp(pdlExampleApp* app);

#endif
