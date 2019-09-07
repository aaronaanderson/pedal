#include "pedal/pedal.hpp"
#include "example_app.hpp"
#include <cmath>

float phase = 0.0f;

void callback(float* out, unsigned buffer, unsigned rate, unsigned channel,
              double time, pdlExampleApp* app) {
    float freq = pdlGetSlider(app, 0);
    for (unsigned i = 0; i < buffer; i += 1) {
        phase += freq / rate;
        if (phase > 1.0f) {
            phase -= 1.0f;
        }
        for (unsigned j = 0; j < channel; j += 1) {
            out[channel * i + j] = std::sin(3.1415926535f * 2.0f * phase);
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
    pdlStartExampleApp(app);
    while (pdlRunExampleApp(app)) {
        pdlUpdateExampleApp(app);
    }
    pdlDeleteExampleApp(app);
}
