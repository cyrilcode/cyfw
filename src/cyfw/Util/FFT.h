#ifndef CYFW_FFT_H
#define CYFW_FFT_H

#include "cyfw/main.h"

namespace cy
{
    class FFT {
    public:
        void fft(const unsigned int band);

        // Values vary over the range [0,1]
        // where 1 means there's a kick happening.
        // Cyril uses very basic peak detection from the FFT
        // TODO: look at using OnsetsDS by Dan Stowell
        // http://onsetsds.sourceforge.net/
        float kick();
        float snare();
        float hihat();

        void audioReceived(float* input, int bufferSize, int numberChannels);
    };

}

#endif //CYFW_FFT_H
