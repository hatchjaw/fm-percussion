/*
  ==============================================================================

    FMOsc.cpp
    Created: 2 Jan 2022 11:59:09am
    Author:  Tommy Rushton

  ==============================================================================
*/

#include "FMOsc.h"

void FMOsc::addModulator(FMOsc *modulator) {
    modulators.add(modulator);
}

void FMOsc::prepareToPlay(juce::dsp::ProcessSpec &spec) {
    this->sampleRate = spec.sampleRate;
    this->envelope.setParameters(0.0f, .01f, 5.f);

    for (auto *modulator: modulators) {
        modulator->prepareToPlay(spec);
    }
}

float FMOsc::computeNextSample() {
    auto sample = 0.f;
    auto modulation = 0.f;

    // Calculate modulation amount.
    // For 1 modulator this will either be basic FM or, if the modulator has modulators of its own, series MMFM.
    // For >1 modulators this will be parallel MMFM.
    // TODO: feedback FM
    for (auto *modulator: modulators) {
        modulation += modulator->computeNextSample();
    }

    sample = (float) (this->amplitude * std::sin(this->currentAngle + modulation));

    this->currentAngle += this->angleDelta;

    return this->envelope.getNextSample() * sample;
}

void FMOsc::computeNextBlock(juce::AudioBuffer<float> &buffer, int startSample, int numSamples) {
    if (this->angleDelta != 0.0) {
        while (--numSamples >= 0) {
            auto currentSample = this->computeNextSample();

            for (auto i = (int) buffer.getNumChannels(); --i >= 0;) {
                buffer.addSample((int) i, startSample, currentSample);
            }

            ++startSample;
        }
    }
}

void FMOsc::reset() {
    this->currentAngle = 0.0;
//    this->envelope.reset();
    for (auto *modulator: modulators) {
        modulator->reset();
    }
}

void FMOsc::setupNote(double sampleRate, double frequency, float noteAmplitude) {
    this->amplitude = noteAmplitude;

    auto cyclesPerSample = frequency / sampleRate;

    angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;

    this->envelope.noteOn();

    this->setupModulators(sampleRate, 550, 10 * noteAmplitude * 550, 5.f);

//    if (this->modulator) {
//        this->modulator->adsr.noteOn();
//    }
}

void FMOsc::setupModulators(double sampleRate, double frequency, double peakDeviation, float decay) {
    for (auto *modulator: modulators) {
        modulator->setupNote(sampleRate, frequency, peakDeviation / frequency);
        modulator->envelope.setParameters(0.0f, 0.0f, decay);
        modulator->envelope.noteOn();
    }
}