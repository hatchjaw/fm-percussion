/*
  ==============================================================================

    FMVoice.cpp
    Created: 1 Jan 2022 8:47:31pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#include "FMVoice.h"
#include "FMBellSound.h"

bool FMVoice::canPlaySound(juce::SynthesiserSound *sound) {
    return dynamic_cast<FMBellSound *>(sound) != nullptr;
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
    this->envelope.noteOn();
}

void FMVoice::stopNote(float velocity, bool allowTailOff) {

}

void FMVoice::controllerMoved(int controllerNumber, int newControllerValue) {

}

void FMVoice::pitchWheelMoved(int newPitchWheelValue) {

}

void FMVoice::prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels) {
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = numOutputChannels;

    this->carrier->prepareToPlay(spec);
    this->envelope.setParameters(0.0f, 0.01f, 5.f);

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
    // Maybe just use the oscillator's envelope
//    this->envelope.applyEnvelopeToBuffer(this->buffer, 0, this->buffer.getNumSamples());

    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel) {
        outputBuffer.addFrom(channel, startSample, this->buffer, channel, 0, numSamples);

        if (!this->envelope.isActive()) {
            this->clearCurrentNote();
            this->carrier->reset();
        }
    }
}
