/*
  ==============================================================================

    FMVoice.h
    Created: 1 Jan 2022 8:47:31pm
    Author:  Tommy Rushton

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include <utility>
#include "../Oscillators/FMOsc.h"
#include "../Envelopes/OADEnv.h"

class FMVoice : public juce::SynthesiserVoice {
public:
    struct Parameters {
        Parameters() = default;

        Parameters(FMOsc::FMMode carrierModeToUse,
                   std::vector<FMOsc::Parameters> modulatorSettingsToUse,
                   OADEnv::Parameters *envParamsToUse)
                : carrierMode(carrierModeToUse),
                  modulatorSettings(std::move(modulatorSettingsToUse)),
                  envParams(envParamsToUse) {
        }

        FMOsc::FMMode carrierMode;
        std::vector<FMOsc::Parameters> modulatorSettings;
        OADEnv::Parameters *envParams;
    };

    ~FMVoice() override;

    void setCarrier(FMOsc *newCarrier);

    bool canPlaySound(juce::SynthesiserSound *sound) override;

    void startNote(int midiNoteNumber,
                   float velocity,
                   juce::SynthesiserSound *sound,
                   int currentPitchWheelPosition) override;

    void stopNote(float velocity, bool allowTailOff) override;

    void controllerMoved(int controllerNumber, int newControllerValue) override;

    void pitchWheelMoved(int newPitchWheelValue) override;

    void prepareToPlay(double sampleRate, int samplesPerBlock, int numOutputChannels);

    void renderNextBlock(juce::AudioBuffer<float> &outputBuffer,
                         int startSample,
                         int numSamples) override;

    void updateModulationAmount(float newModAmount);

protected:
    FMOsc *carrier;

    bool isPrepared{false};

private:
    juce::AudioBuffer<float> buffer;
};
