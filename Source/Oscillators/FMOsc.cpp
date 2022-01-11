/*
  ==============================================================================

    FMOsc.cpp
    Created: 2 Jan 2022 11:59:09am
    Author:  Tommy Rushton

  ==============================================================================
*/

#include "FMOsc.h"

FMOsc::FMOsc(FMOsc::FMMode modeToSet) : mode(modeToSet) {}

void FMOsc::addModulator(FMOsc modulatorToAdd) {
    modulators.push_back(modulatorToAdd);
}

void FMOsc::prepareToPlay(juce::dsp::ProcessSpec &spec) {
    this->sampleRate = spec.sampleRate;

    for (auto &modulator: modulators) {
        modulator.prepareToPlay(spec);
    }
}

float FMOsc::computeNextSample() {
    auto sample = 0.f;
    auto modulation = 0.f;

    // Calculate modulation amount.
    // For 0 modulators this is an unmodulated oscillator.
    // For 1 modulator this will either be basic FM or, if the modulator has modulators of its own, series MMFM.
    // For >1 modulators this will be parallel MMFM.
    for (auto &modulator: modulators) {
        modulation += modulator.computeNextSample();
    }

    switch (mode) {
        case LINEAR:
            sample = (float) (this->amplitude *
                              std::sin(this->currentAngle + modulation + (feedback * modulationAmount * prevSample)));
            break;
        case EXPONENTIAL:
            sample = (float) (this->amplitude *
                              std::sin(this->currentAngle * pow(2, modulation) +
                                       (feedback * modulationAmount * prevSample)));
            break;
        default:
            jassertfalse;
    }

    this->currentAngle += this->angleDelta;
    prevSample = sample;
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

    for (auto &modulator: modulators) {
        modulator.reset();
    }
}

void FMOsc::setupNote(double sampleRate, double frequency, float noteAmplitude) {
    this->amplitude = noteAmplitude;

    auto cyclesPerSample = frequency / sampleRate;

    angleDelta = cyclesPerSample * juce::MathConstants<double>::twoPi;

    this->envelope.noteOn();

    for (auto &m: modulators) {
        auto modFreq = m.modMode == PROPORTIONAL ? frequency * m.modulationFrequencyRatio : m.modulationFrequency;
        // modulation index, I = d/m; d, peak deviation; m, modulation frequency;
        auto modulationIndex = noteAmplitude * modulationAmount * (m.peakDeviation / modFreq);
        m.setupNote(sampleRate, modFreq, static_cast<float>(modulationIndex));
    }
}

bool FMOsc::isActive() {
    return this->envelope.isActive();
}

void FMOsc::setEnvelope(OADEnv::Parameters &newParams) {
    this->envelope.setParameters(newParams);
    for (auto &modulator: modulators) {
        if (!modulator.envelopeSet) {
            modulator.setEnvelope(newParams);
        }
    }
}

void FMOsc::setModulationAmount(float newModulationAmount) {
    this->modulationAmount = newModulationAmount;
    for (auto &m: modulators) {
        m.setModulationAmount(newModulationAmount);
    }
}

