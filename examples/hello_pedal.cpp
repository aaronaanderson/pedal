#include "pedal/pedal.hpp"
#include "example_app.hpp"
#include <cmath>

float phase = 0.0f;
float envelope = 0.0f;

void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
    float freq = pdlGetSlider(app, 0);
    bool loud = pdlGetToggle(app, 0);
    bool trig = pdlGetTrigger(app, 0);
    float amp = loud? 1.0f : 0.5f;
    if (trig) envelope = 0.0f;
    for (unsigned i = 0; i < buffer; i += 1) {
        phase += freq / rate;
        envelope += 0.0001f * (1.0f - envelope);
        if (phase > 1.0f) {
            phase -= 1.0f;
        }
        for (unsigned j = 0; j < channel; j += 1) {
            out[channel * i + j] = amp * envelope * std::sin(3.1415926535f * 2.0f * phase);
        }
    }
}

int main() {
    pdlHello();
    pdlExampleApp* app = pdlInitExampleApp(callback);
    if (!app) {
        return 1;
    }
    pdlAddSlider(app, 0, "freq", 0.0f, 1000.0f, 440.0f);
    pdlAddToggle(app, 0, "loud", false);
    pdlAddTrigger(app, 0, "trigger");
    pdlStartExampleApp(app);
    while (pdlRunExampleApp(app)) {
        pdlUpdateExampleApp(app);
    }
    pdlDeleteExampleApp(app);
}
