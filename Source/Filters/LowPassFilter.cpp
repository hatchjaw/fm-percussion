/*
  ==============================================================================

    LowPassFilter.cpp
    Created: 11 Jan 2022 10:26:55pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#include "LowPassFilter.h"

LowPassFilter::LowPassFilter() {
    clear();
}

void LowPassFilter::clear() noexcept {
    for (int i = 0; i < 16; ++i) {
        xPrev.push_back(0.0);
        yPrev.push_back(0.0);
    }
}

/**
 * Filter a sample.
 *
 * y[n] = b0*x[n] + b1*x[n-1] - a1*y[n-1] + ... + bN*x[n-N] - aN*y[n-N]
 *
 * @param input The sample to filter
 * @return The filtered sample
 */
float LowPassFilter::processSample(float input) noexcept {
    double output = (double) input * b0;

    for (unsigned long i = 0; i < 16; ++i) {
        output += bCoeff[i] * xPrev[i] - aCoeff[i] * yPrev[i];
    }

    for (unsigned long i = 15; i > 0; --i) {
        xPrev[i] = xPrev[i - 1];
        yPrev[i] = yPrev[i - 1];
    }

    xPrev[0] = (double) input;
    yPrev[0] = output;

//    if (std::fabs(output) < 1.0e-6) {
//        output = 0.0;
//    }

    return (float) output;
}

void LowPassFilter::processBlock(juce::AudioBuffer<float> buffer, int numSamples) {
    for (int channel = 0; channel < buffer.getNumChannels(); ++channel) {
        for (int sample = 0; sample < numSamples; ++sample) {

        }
    }
}
