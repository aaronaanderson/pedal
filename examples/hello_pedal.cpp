#include "pedal/pedal.hpp"
#include "example_app.hpp"
#include <cmath>

#include "pedal/TSine.hpp"
#include "pedal/TSaw.hpp"
#include "pedal/TSquare.hpp"
#include "pedal/TTriangle.hpp"
#include "pedal/TSaw.hpp"
#include "pedal/CTEnvelope.hpp"

//loat envelope = 0.0f;
TSquare square;
TSine saw;
CTEnvelope envelope;
void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
    saw.setFrequency(pdlGetSlider(app, 0));
    bool trigger = pdlGetToggle(app, 0);
   // bool trigger = pdlGetTrigger(app, 0);
    envelope.setTriger(trigger);
    float mx, my; pdlGetCursorPos(app, &mx, &my);
    //square.setDutyCycle(mx*0.5);

    //if (trig) envelope = 0.0f;
    for (unsigned i = 0; i < buffer; i += 1) {
        //envelope += 0.0001f * (1.0f - envelope);
        float currentSample = saw.generateSample();
        currentSample *= envelope.generateSample();
        for (unsigned j = 0; j < channel; j += 1) {
          out[channel * i + j] = currentSample;
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
