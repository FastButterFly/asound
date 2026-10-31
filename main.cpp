#include <iostream>
#include <fstream>

double PI = 3.14159;

#include <cmath>

struct Frequency {
    double hz;
    double amplitude;
};

int main() {

    std::fstream stream{"audio.pcm", std::ios::binary | std::ios::out};

    short a = 32767;

    long long fs = 44100;

    double volume = 1;

    Frequency freqs[] = {
    {   .hz = 440, .amplitude = .8  },
    {   .hz = 880, .amplitude = .3  }, 
    {   .hz = 550, .amplitude = .5  }, 
    {   .hz = 670, .amplitude = .15 } 
    };

    for(double n = 0; n < fs * 60; n++) {
        double sample = 0.0;

        for(auto& freq : freqs) {
            sample += std::sin(2 * PI * freq.hz * (n / fs)) * freq.amplitude;
        }

        short probel = ((a * volume) / (sizeof(freqs) / sizeof(Frequency))) * sample;
        short prober = -probel;

        stream.write(reinterpret_cast<const char*>(&probel), sizeof(probel));
        stream.write(reinterpret_cast<const char*>(&prober), sizeof(prober));
    }

    return 0;
}