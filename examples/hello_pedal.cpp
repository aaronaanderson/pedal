#include "pedal/pedal.hpp"
#include "example_app.hpp"
#include <cmath>

#include "pedal/TSine.hpp"
#include "pedal/TSaw.hpp"
#include "pedal/TSquare.hpp"
#include "pedal/TTriangle.hpp"
//#include "pedal/Saw.hpp"

float envelope = 0.0f;
TSquare square;

void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
    square.setFrequency(pdlGetSlider(app, 0));
    bool loud = pdlGetToggle(app, 0);
    bool trig = pdlGetTrigger(app, 0);
    float mx, my; pdlGetCursorPos(app, &mx, &my);
    square.setDutyCycle(mx*0.5);
    float amp = loud? 1.0f : 0.5f;
    if (trig) envelope = 0.0f;
    for (unsigned i = 0; i < buffer; i += 1) {
        envelope += 0.0001f * (1.0f - envelope);
        float currentSample = square.generateSample() * envelope;
        for (unsigned j = 0; j < channel; j += 1) {
          out[channel * i + j] = amp * currentSample;
        }
    }
}

int main() {
    pdlHello();
    pdlExampleApp* app = pdlInitExampleApp(callback);
    if (!app) {
      return 1;
    }
    
    // Add your GUI elements here
    pdlAddSlider(app, 0, "freq", 0.0f, 1000.0f, 440.0f);
    pdlAddToggle(app, 0, "loud", false);
    pdlAddTrigger(app, 0, "trigger");

    pdlStartExampleApp(app);
    while (pdlRunExampleApp(app)) {
        pdlUpdateExampleApp(app);
    }
    pdlDeleteExampleApp(app);
}
