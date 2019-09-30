#include "pedal/pedal.hpp"
#include "example_app.hpp"
#include <cmath>

#include "pedal/TSine.hpp"

float envelope = 0.0f;
TSine sine;

void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
<<<<<<< HEAD
    //float freq = pdlGetSlider(app, 0);
    sine.setFrequency(pdlGetSlider(app, 0));
=======
    // Get values from window IO and GUI
    float freq = pdlGetSlider(app, 0);
>>>>>>> 21834aae168ddc1645bd7901a57a2008ca0f3bb4
    bool loud = pdlGetToggle(app, 0);
    bool trig = pdlGetTrigger(app, 0);
    float mx, my; pdlGetCursorPos(app, &mx, &my);

    float amp = loud? 1.0f : 0.5f;
    if (trig) envelope = 0.0f;
<<<<<<< HEAD

=======
    freq *= (0.5f + mx);
>>>>>>> 21834aae168ddc1645bd7901a57a2008ca0f3bb4
    for (unsigned i = 0; i < buffer; i += 1) {
        envelope += 0.0001f * (1.0f - envelope);
<<<<<<< HEAD
        for (unsigned j = 0; j < channel; j += 1) {
            out[channel * i + j] = amp * envelope * sine.generateSample();
=======
        if (phase > 1.0f) {
            phase -= 1.0f;
        }
        float s = std::sin(3.1415926535f * 2.0f * phase);
        for (unsigned j = 0; j < channel; j += 1) {
            out[channel * i + j] = amp * envelope * s;
>>>>>>> 21834aae168ddc1645bd7901a57a2008ca0f3bb4
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
