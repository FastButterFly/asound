#include <iostream>
#include <fstream>

double PI = 3.14159;

#include <cmath>

struct FreqWaveSqrtFuncCache {
    int after_probes_change;
    bool state;
    unsigned int current_probe_n;
    bool bipolar;
};

enum class FrequencyFunction {
    SINE,
    SQUARE_WAVE
};

struct Frequency {
    double hz;
    double amplitude;
    FrequencyFunction function;
    FreqWaveSqrtFuncCache waveSqrtCache;
};

int main() {

    std::fstream stream{"audio.pcm", std::ios::binary | std::ios::out};

    short a = 32767;

    long long fs = 44100;

    double volume = 1.0;

    Frequency freqs[] = {
    {   .hz = 440, .amplitude = .8, .function = FrequencyFunction::SINE  },
    {   .hz = 880, .amplitude = .3, .function = FrequencyFunction::SINE  }, 
    {   .hz = 550, .amplitude = .5, .function = FrequencyFunction::SINE  }, 
    {   .hz = 670, .amplitude = .15, .function = FrequencyFunction::SINE } 
    };

    for(double n = 0; n < fs * 60; n++) {
        double sample = 0.0;

        for(auto& freq : freqs) {
            if(freq.function == FrequencyFunction::SINE) {
                sample += std::sin(2 * PI * freq.hz * (n / fs)) * freq.amplitude;
            } else if(freq.function == FrequencyFunction::SQUARE_WAVE) {

                if(freq.waveSqrtCache.after_probes_change == -1) {
                    freq.waveSqrtCache.after_probes_change = fs / freq.hz;
                }

                if(freq.waveSqrtCache.state) {
                    sample += 1.0 * freq.amplitude;
                } else {
                    if(freq.waveSqrtCache.bipolar) {
                        sample += -1.0 * freq.amplitude;
                    } else {
                        sample += 0.0;
                    }
                }

                freq.waveSqrtCache.current_probe_n++;
                if(freq.waveSqrtCache.current_probe_n == freq.waveSqrtCache.after_probes_change) {
                    freq.waveSqrtCache.state = !freq.waveSqrtCache.state;
                }
            }
        }

        short probel = ((a * volume) / (sizeof(freqs) / sizeof(Frequency))) * sample;
        short prober = ((a * 0.95 * volume) / (sizeof(freqs) / sizeof(Frequency))) * sample;

        stream.write(reinterpret_cast<const char*>(&probel), sizeof(probel));
        stream.write(reinterpret_cast<const char*>(&prober), sizeof(prober));
    }

    return 0;
}