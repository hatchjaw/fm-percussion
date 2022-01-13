/*
  ==============================================================================

    FMVoice.cpp
    Created: 1 Jan 2022 8:47:31pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#include "FMVoice.h"
#include "FMSound.h"

FMVoice::~FMVoice() {
    // I'm certain there's a better way than this... I think carrier should be
    // a unique_ptr, but I don't know how to use those properly.
    delete carrier;
}

bool FMVoice::canPlaySound(juce::SynthesiserSound *sound) {
    return dynamic_cast<FMSound *>(sound) != nullptr;
}

void FMVoice::setCarrier(FMOsc *newCarrier) {
    this->carrier = newCarrier;
}

void FMVoice::startNote(
        int midiNoteNumber,
        float velocity,
        juce::SynthesiserSound *sound,
        int currentPitchWheelPosition
) {
    this->carrier->setupNote(getSampleRate(), juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber), velocity);
}

void FMVoice::stopNote(float velocity, bool allowTailOff) {

}

void FMVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void FMVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void FMVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels) {
    this->clearCurrentNote();
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numOutputChannels;

    this->carrier->prepareToPlay(spec);

    this->isPrepared = true;
}

void FMVoice::renderNextBlock(juce::AudioBuffer<float> &outputBuffer, int startSample, int numSamples) {
    jassert(this->isPrepared);

    if (!this->isVoiceActive()) {
        return;
    }

    // Prevent discontinuities by writing to a temp buffer...
    this->buffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    this->buffer.clear();

    juce::dsp::AudioBlock<float> audioBlock{this->buffer};

    this->carrier->computeNextBlock(this->buffer, 0, this->buffer.getNumSamples());

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, this->buffer, channel, 0, numSamples);

        if (!this->carrier->isActive()) {
            this->clearCurrentNote();
            this->carrier->reset();
        }
    }
}

void FMVoice::updateModulationAmount(float newModAmount) {
    this->carrier->setModulationAmount(newModAmount);
}