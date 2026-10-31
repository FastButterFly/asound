#include <iostream>
#include <fstream>

double PI = 3.14159;

#include <cmath>

int main() {

    std::fstream stream{"audio.pcm", std::ios::binary | std::ios::out};

    short a = 32767;

    int f = 100;
    int f2 = 200;
    int f3 = 440;
    long long fs = 44100;

    double freqs[] = { 440, 1320, 880, 392, 261.63, 261.63 };

    for(double n = 0; n < fs * 60; n++) {
        double sample = 0.0;

        for(auto& freq : freqs) {
            sample += std::sin(2 * PI * freq * (n / fs));
        }

        short probel = (a / (sizeof(freqs) / sizeof(double))) * sample;
        short prober = -probel;

        stream.write(reinterpret_cast<const char*>(&probel), sizeof(probel));
        stream.write(reinterpret_cast<const char*>(&prober), sizeof(prober));
    }

    return 0;
}