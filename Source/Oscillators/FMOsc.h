/*
  ==============================================================================

    FMOsc.h
    Created: 2 Jan 2022 11:59:09am
    Author:  Tommy Rushton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../Envelopes/OADEnv.h"

class FMOsc {
public:
    void addModulator(FMOsc *modulator);

    void prepareToPlay(juce::dsp::ProcessSpec &spec);

    void setupNote(double sampleRate, double frequency, float noteAmplitude);

    void setupModulators(double sampleRate, double frequency, double peakDeviation, float decay);

    float computeNextSample();

    void computeNextBlock(juce::AudioBuffer<float> &buffer, int startSample, int numSamples);

    void reset();

    enum MMFMMode {
        PARALLEL,
        SERIES
    };
protected:

private:
    juce::OwnedArray<FMOsc> modulators;
    MMFMMode mode{ SERIES };

    double sampleRate{0.0};

    double currentAngle{0.0};
    double angleDelta{0.0};
    double amplitude{1.0};

    OADEnv envelope;
};